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

struct NormalMapVertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW  : NORMAL;
	float2 Tex     : TEXCOORD;
	float3 ViewDirection : VIEWDIR;

	float4 ShadowPosH : POS_SHADOW;
	float3 TangentW  : TANGENTW;
	float3x3 TBN : TANGENT;
};

float4 main(NormalMapVertexOut pin) : SV_TARGET
{
	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	texColor = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
	
	// Gamma Correction
	// Gamma Space -> Linear Space
	// ��� ������ ������ ���� �������� �̷������ �Ѵ�..
	texColor.rgb = pow(texColor.rgb, 2.2f);
	
	// �ؽ�ó ǥ���� ���� ������ 0.1 ���� ������ �ȼ��� ��������� ���� ��� ����.
	clip(texColor.a - 0.1f);
	
	//
	// Normal mapping
	//
	float3 normalMapSample = float3(0.0f, 0.0f, 0.0f);
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);

	normalMapSample = 2.0f * gNormalMap.Sample(samWrapMinLinear, pin.Tex).rgb - 1.0f;
	bumpedNormalW = mul(normalMapSample, pin.TBN);

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
			ComputeDirectionalLight(gMaterial, gDirLights[i], bumpedNormalW, pin.ViewDirection,
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
			ComputePointLight(gMaterial, gPointLights[i], pin.PosW, bumpedNormalW, pin.ViewDirection,
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
			ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, bumpedNormalW, pin.ViewDirection,
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
	
	float3 incident = -pin.ViewDirection;
	float3 reflectionVector = reflect(incident, bumpedNormalW);
	float4 reflectionColor = gCubeMap.Sample(samWrapAnisotropic, reflectionVector);

	litColor += gMaterial.Reflect * reflectionColor;

	// Gamma Correction
	// Normal Map�� ������������ ���..
	// Diffuse Map�� ������������ ���..
	//litColor.rgb = pow(litColor.rgb, 1 / 2.2);
	
	return litColor;
}