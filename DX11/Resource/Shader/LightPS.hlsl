#include "LightHelper.fx"

cbuffer cbLightList : register(b0)
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[16];
	SpotLight gSpotLights[16];

	uint gPointLightCount;
	uint gSpotLightCount;
};

cbuffer cbCamera : register(b1)
{
    float3 gEyePosW : packoffset(c0);
};

cbuffer cbMaterialList : register(b2)
{
	Material gMaterials[20];
}

cbuffer cbTexViewProj : register(b3)
{
    float4x4 gViewProjTex : packoffset(c0);
}

Texture2D AlbedoSRV		 : register(t0);
Texture2D NormalSRV		 : register(t1);
Texture2D PositionSRV	 : register(t2);
Texture2D ShadowSRV		 : register(t3);

Texture2D SsaoSRV		 : register(t4);

// 공용 TextureMap
Texture2D gShadowMap : register(t16);

SamplerComparisonState gShadowSam : register(s0);
SamplerState samWrapMinLinear : register(s1);

struct VertexIn
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

float4 main(VertexIn pin) : SV_TARGET
{
	float4 albedo = AlbedoSRV.Sample(samWrapMinLinear, pin.Tex);
	float4 normal = NormalSRV.Sample(samWrapMinLinear, pin.Tex);
	float4 position = PositionSRV.Sample(samWrapMinLinear, pin.Tex);
    float4 shadow = ShadowSRV.Sample(samWrapMinLinear, pin.Tex);
    float4 ssao = mul(gViewProjTex, float4(position.xyz, 1.0f));
	
    // Gamma Correction
	// Gamma Space -> Linear Space
	// 모든 라이팅 연산은 선형 공간에서 이루어져야 한다..
	albedo.rgb = pow(albedo.rgb, 2.2f);

	float4 litColor = albedo;

	// Start with a sum of zero.
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// View Direction
	float3 ViewDirection = gEyePosW - position.xyz;
	ViewDirection = normalize(ViewDirection);

	// 현재 픽셀의 Shadow 값..
    
    float shadows = CalcShadowFactor(gShadowSam, gShadowMap, float3(shadow.xyz));
	
	// 현재 픽셀의 SSAO 값..
    ssao /= ssao.w;
    float ambientAccess = SsaoSRV.Sample(samWrapMinLinear, ssao.xy, 0.0f).r;
	
	// 현재 픽셀의 Material ID..
    uint matID = round(position.w);

	float4 A, D, S;
	
    if (shadow.w < 1.0f)
	{
		// Directional Light
		[unroll]
		for (uint i = 0; i < 3; ++i)
		{
			ComputeDirectionalLight(gMaterials[matID], gDirLights[i], float3(normal.xyz), ViewDirection,
				A, D, S);

            ambient += ambientAccess * A;
            diffuse += D;
            spec += S;
        }


		// Point Light
		if (gPointLightCount > 0)
		{
			[unroll]
			for (uint i = 0; i < gPointLightCount; ++i)
			{
				ComputePointLight(gMaterials[matID], gPointLights[i], float3(position.xyz), float3(normal.xyz), ViewDirection,
					A, D, S);

                ambient += ambientAccess * A;
                diffuse += D;
                spec += S;
            }
		}

		// Spot Light
		if (gSpotLightCount > 0)
		{
			[unroll]
			for (uint i = 0; i < gSpotLightCount; ++i)
			{
				ComputeSpotLight(gMaterials[matID], gSpotLights[i], float3(position.xyz), float3(normal.xyz), ViewDirection,
					A, D, S);

                ambient += ambientAccess * A;
                diffuse += D;
                spec += S;
            }
		}
		
		// Modulate with late add.
		litColor = albedo * (ambient + diffuse) + spec;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterials[matID].Diffuse.a * albedo.a;

	// Gamma Correction
	// Normal Map은 선형공간에서 출력..
	// Diffuse Map은 감마공간에서 출력..
    if (normal.w < 1.0f)
        litColor.rgb = pow(litColor.rgb, 1.0f / 2.2f);

    return litColor;
  //return float4(normal.xyz, 1.0f);
}