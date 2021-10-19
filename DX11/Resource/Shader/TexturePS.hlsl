#include "PixelHeader.hlsli"

cbuffer cbLights : register(b0)
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

cbuffer cbMaterial : register(b1)
{
	Material gMaterial;
}

struct TexVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW  : NORMAL;
	float2 Tex     : TEXCOORD;
	float3 ViewDirection : VIEWDIR;

	float4 ShadowPosH : POS_SHADOW;
	float4 SsaoPosH : POS_SSAO;
};


float4 main(TexVertexOut pin) : SV_TARGET
{
	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	texColor = gDiffuseMap.Sample(samWrapAnisotropic, pin.Tex);

	// Gamma Correction
	// Gamma Space -> Linear Space
	// ��� ������ ������ ���� �������� �̷������ �Ѵ�..
	texColor.rgb = pow(texColor.rgb, 2.2f);

	// �ؽ�ó ǥ���� ���� ������ 0.1 ���� ������ �ȼ��� ��������� ���� ��� ����.
	clip(texColor.a - 0.1f);

	//
	// Shadow
	//
	float shadow[3] = { 1.0f, 1.0f, 1.0f };
	shadow[0] = CalcShadowFactor(gShadowSam, gShadowMap, pin.ShadowPosH);

	//
	// Lighting.
	//
	float4 litColor = texColor;

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	// Sum the light contribution from each light source.  
	{
		[unroll]
		for (int i = 0; i < 3; ++i)
		{
			ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, pin.ViewDirection,
				A, D, S);

			ambient += A;
			diffuse += shadow[0] * D;
			spec += shadow[0] * S;
		}
	}

	{
		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			ComputePointLight(gMaterial, gPointLights[i], pin.PosW, pin.NormalW, pin.ViewDirection,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	{
		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, pin.NormalW, pin.ViewDirection,
				A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	// Modulate with late add.
	litColor = texColor * (ambient + diffuse) + spec;

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	// Gamma Correction
	// Normal Map�� ������������ ���..
	// Diffuse Map�� ������������ ���..
	litColor.rgb = pow(litColor.rgb, 1.0f / 2.2f);

	return litColor;
}