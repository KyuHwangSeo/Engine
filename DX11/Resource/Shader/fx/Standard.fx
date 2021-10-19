//=============================================================================
// Basic.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Basic effect that currently supports transformations, lighting, and texturing.
//=============================================================================

#include "LightHelper.fx"

bool gUseShadow = true;

cbuffer cbPerFrame : register(b0)
{
	float3 gEyePosW;

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor;
};

cbuffer cbLights : register(b1)
{
	DirectionalLight gDirLights[3];
	PointLight gPointLights[4];
	SpotLight gSpotLights[4];
};

cbuffer cbPerObject : register(b2)
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
};

cbuffer cbMaterial : register(b3)
{
	Material gMaterial;
}

cbuffer cbShadow : register(b4)
{
	float4x4 gShadowTransform;
};

cbuffer cbSkinned : register(b5)
{
	float4x4 gBoneTransforms[96];
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;
Texture2D gShadowMap;
TextureCube gCubeMap;

// Shadow 전용 Sampler
SamplerComparisonState gShadowSam
{
	Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;

	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = BORDER;

	BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComparisonFunc = LESS_EQUAL;
};

SamplerState samClampMinLinear
{
	Filter = MIN_MAG_MIP_LINEAR;

	MaxAnisotropy = 4;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState samWrapAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

// shadow
SamplerState samWrapMinLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct PosVertexIn
{
	float3 PosL  : POSITION;
};

struct ColorVertexIn
{
	float3 PosL  : POSITION;
	float4 Color : COLOR;
};

struct TexVertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
};

struct NormalMapVertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct SkinVertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
	uint4 BoneIndices : BONEINDICES;
	float3 BoneWeights : WEIGHTS;
};

RasterizerState Depth
{
	// [MSDN]
	// 현재 출력 병합기 단계에 묶인 깊이 버퍼의 형식이 UNORM이거나
	// 출력 병합기 단계에 아무런 깊이 버퍼도 묶여 있지 않다면,
	// 편향치는 다음과 같이 계산된다.
	//
	// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
	//
	// 여기서 r은 깊이 버퍼 형식을 float32로 변환했을 때 표현 가능한,
	// 0보다 큰 최솟값이다.
	// [MSDN]
	//
	// 24비트 깊이 버퍼의 경우 r = 1 / 2^24이다.
	// 예 : DepthBias = 100000 ==> 실제 DepthBias = 100000/2^24 = .006
	//
	// 이 값들은 장면의 특성에 크게 의존하므로, 독자의 장면에
	// 잘 맞는 값들은 실험에 거쳐서 구해야 할 것이다.

	DepthBias = 100000;
	DepthBiasClamp = 0.0f;
	SlopeScaledDepthBias = 0.005f;
}; 

RasterizerState NoCull
{
	CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
	DepthFunc = LESS_EQUAL;
};

struct PosVertexOut
{
	float4 PosH  : SV_POSITION;
	float3 PosL : POSITION;
};

struct ColorVertexOut
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

struct ShadowVertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float2 Tex     : TEXCOORD;
	float3 ViewDirection : VIEWDIR;

	float3 NormalW  : NORMAL;
	float3 TangentW  : TANGENTW;
	float3x3 TBN : TANGENT;
	float4 ShadowPosH : POS_SHADOW;
};

// NormalMap VS
VertexOut NormalMapVS(NormalMapVertexIn vin)
{
	VertexOut vout;

	// Transform to world space space.
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.NormalW = normalize(vout.NormalW);

	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	// Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
	float3 N = vout.NormalW;
	float3 T = normalize(vout.TangentW - dot(vout.TangentW, N) * N);
	float3 B = cross(N, T);

	vout.TBN = float3x3(T, B, N);

	vout.ShadowPosH = mul(float4(vin.PosL + (vin.NormalL * 0.1f), 1.0f), gShadowTransform);

	return vout;
}

// Skinning VS
VertexOut SkinnedVS(SkinVertexIn vin)
{
	VertexOut vout;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vin.BoneWeights.x;
	weights[1] = vin.BoneWeights.y;
	weights[2] = vin.BoneWeights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
		normalL += weights[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
	}

	// 세계 공간 변환
	vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);
	vout.NormalW = normalize(vout.NormalW);

	//동차 공간으로 변환
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);

	// 정점 특성 출력(삼각형 표면을 따라 보간
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	vout.ShadowPosH = mul(float4(posL + (vin.NormalL * 0.1f), 1.0f), gShadowTransform);

	return vout;
}

// Texture VS
VertexOut TexVS(TexVertexIn vin)
{
	VertexOut vout;

	// 세계 공간 변환
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.NormalW = normalize(vout.NormalW);

	//동차 공간으로 변환
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);

	// 정점 특성 출력(삼각형 표면을 따라 보간
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	vout.ShadowPosH = mul(float4(vin.PosL + (vin.NormalL * 0.1f), 1.0f), gShadowTransform);

	return vout;
}

// Normal VS
VertexOut BasicVS(TexVertexIn vin)
{
	VertexOut vout;

	// 세계 공간 변환
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.NormalW = normalize(vout.NormalW);

	//동차 공간으로 변환
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);

	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	vout.ShadowPosH = mul(float4(vin.PosL + (vin.NormalL * 0.1f), 1.0f), gShadowTransform);

	return vout;
}

// Color VS
ColorVertexOut ColorVS(ColorVertexIn vin)
{
	ColorVertexOut vout;

	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

	// Just pass vertex color into the pixel shader.
	vout.Color = vin.Color;

	//vout.Color.x = 1.0f;
	//vout.Color.y = 0;
	//vout.Color.z = 0;
	//vout.Color.w = 1.0f;

	return vout;
}

// CubeMap VS
PosVertexOut PosVS(PosVertexIn vin)
{
	PosVertexOut vout;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj).xyww;

	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;

	return vout;
}

// Shadow Normal VS
ShadowVertexOut ShadowNoramlVS(TexVertexIn vin)
{
	ShadowVertexOut vout;

	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

// Shadow Skin VS
ShadowVertexOut ShadowSkinVS(SkinVertexIn vin)
{
	ShadowVertexOut vout;

	float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	weights[0] = vin.BoneWeights.x;
	weights[1] = vin.BoneWeights.y;
	weights[2] = vin.BoneWeights.z;
	weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

	float3 posL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
		posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
	}
	
	vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	return vout;
}

ShadowVertexOut ShadowDebugVS(TexVertexIn vin)
{
	ShadowVertexOut vout;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.Tex = vin.Tex;

	return vout;
}

float4 PS(VertexOut pin,
	uniform int gDirLightCount,
	uniform int gPointLightCount,
	uniform int gSpotLightCount,
	uniform int gSamplerState,
	uniform bool gUseLight,
	uniform bool gUseTexure,
	uniform bool gUseNormalMap,
	uniform bool gUseReflection,
	uniform bool gUseShadowMap) : SV_Target
{
	// Default to multiplicative identity.
	float4 texColor = float4(1, 1, 1, 1);
	if (gUseTexure)
	{
		switch (gSamplerState)
		{
		case 0:
			texColor = gDiffuseMap.Sample(samWrapAnisotropic, pin.Tex);
			break;
		case 1:
			texColor = gDiffuseMap.Sample(samClampMinLinear, pin.Tex);
			break;
		case 2:
			texColor = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
			break;
		}
	}

	// Gamma Correction
	// Gamma Space -> Linear Space
	// 모든 라이팅 연산은 선형 공간에서 이루어져야 한다..
	texColor.rgb = pow(texColor.rgb, 2.2);
	

	// 텍스처 표본의 알파 성분이 0.1 보다 작으면 픽셀을 폐기함으로 성능 향상에 도움.
	clip(texColor.a - 0.1f);

	//
	// Normal mapping
	//
	float3 normalMapSample = float3(0.0f, 0.0f, 0.0f);
	float3 bumpedNormalW = float3(0.0f, 0.0f, 0.0f);

	if (gUseNormalMap)
	{
		normalMapSample = 2.0f * gNormalMap.Sample(samWrapMinLinear, pin.Tex).rgb - 1.0f;
		bumpedNormalW = mul(normalMapSample, pin.TBN);
	}

	//
	// Shadow
	//

	float shadow[3] = { 1.0f, 1.0f, 1.0f };
	if (gUseShadowMap)
	{
		shadow[0] = CalcShadowFactor(gShadowSam, gShadowMap, pin.ShadowPosH);
	}

	if (gUseShadow == false)
	{
		shadow[0] = 1.0f;
	}

	//
	// Lighting.
	//

	float4 litColor = texColor;

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 A, D, S;

	if (gDirLightCount > 0)
	{
		// Sum the light contribution from each light source.  
		[unroll]
		for (int i = 0; i < gDirLightCount; ++i)
		{
			if (gUseNormalMap)
			{
				ComputeDirectionalLight(gMaterial, gDirLights[i], bumpedNormalW, pin.ViewDirection,
					A, D, S);
			}
			else
			{
				ComputeDirectionalLight(gMaterial, gDirLights[i], pin.NormalW, pin.ViewDirection,
					A, D, S);
			}

			ambient += A;
			diffuse += shadow[0] * D;
			spec += shadow[0] * S;
		}
	}

	if (gPointLightCount > 0)
	{
		[unroll]
		for (int i = 0; i < gPointLightCount; ++i)
		{
			if (gUseNormalMap)
			{
				ComputePointLight(gMaterial, gPointLights[i], pin.PosW, bumpedNormalW, pin.ViewDirection,
					A, D, S);
			}
			else
			{
				ComputePointLight(gMaterial, gPointLights[i], pin.PosW, pin.NormalW, pin.ViewDirection,
					A, D, S);
			}

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	if (gSpotLightCount > 0)
	{
		[unroll]
		for (int i = 0; i < gSpotLightCount; ++i)
		{
			if (gUseNormalMap)
			{
				ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, bumpedNormalW, pin.ViewDirection,
					A, D, S);
			}
			else
			{
				ComputeSpotLight(gMaterial, gSpotLights[i], pin.PosW, pin.NormalW, pin.ViewDirection,
					A, D, S);
			}

			ambient += A;
			diffuse += D;
			spec += S;
		}
	}

	// Modulate with late add.
	if (gUseLight)
	{
		litColor = texColor * (ambient + diffuse) + spec;
	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

	if (gUseReflection)
	{
		float3 incident = -pin.ViewDirection;
		float3 reflectionVector = reflect(incident, pin.NormalW);
		float4 reflectionColor = gCubeMap.Sample(samWrapAnisotropic, reflectionVector);

		litColor += gMaterial.Reflect * reflectionColor;
	}
	
	// Gamma Correction
	// Normal Map은 선형공간에서 출력..
	// Diffuse Map은 감마공간에서 출력..
	if (gUseNormalMap == false)
	{
		litColor.rgb = pow(litColor.rgb, 1 / 2.2);
	}

	return litColor;
}

// CubeMap PS
float4 CubeMapPS(PosVertexOut pin) : SV_Target
{
	return gCubeMap.Sample(samWrapMinLinear, pin.PosL);
}

// ShadowMap PS
void ShadowMapPS(ShadowVertexOut pin)
{
	float4 diffuse = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
	clip(diffuse.a - 0.5f);
}

// ShadowDebug PS
float4 ShadowDebugPS(ShadowVertexOut pin) : SV_Target
{
	float4 color = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
	return float4(color.xxx, 1.0f);
}

// Color PS
float4 ColorPS(ColorVertexOut pin) : SV_Target
{
	//float4 test;
	//test.r = 1.0f;
	//test.g = 0.0f;
	//test.b = 1.0f;
	//test.a = 1.0f;
	//return test;

	//pin.Color.x = pin.PosH.z;
	//pin.Color.y = pin.PosH.z;
	//pin.Color.z = pin.PosH.z;

	//pin.Color.x = 0.5f;
	//pin.Color.y = 0.5f;
	//pin.Color.z = 0.5f;

	return pin.Color;
}


/////////////////////////////////////////////////////////////////////
// Color Tech
/////////////////////////////////////////////////////////////////////
technique11 ColorTech
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, ColorVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, ColorPS()));
	}
}

/////////////////////////////////////////////////////////////////////
// Normal Tech
/////////////////////////////////////////////////////////////////////
technique11 BasicLight1
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(1, 4, 4, 0, true, false, false, false, false)));
	}
}

technique11 BasicLight2
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 0, true, false, false, false, false)));
	}
}

technique11 BasicLight3
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 0, true, false, false, false, true)));
	}
}

/////////////////////////////////////////////////////////////////////
// Texture Tech
/////////////////////////////////////////////////////////////////////
technique11 TexLight1
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(0, 0, 0, 0, false, true, false, false, false)));
	}
}

technique11 TexLight2
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 0, true, true, false, false, false)));
	}
}

technique11 TexLight3
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 0, true, true, false, false, true)));
	}
}

/////////////////////////////////////////////////////////////////////
// Skinning Tech
/////////////////////////////////////////////////////////////////////
technique11 SkinnedTex1
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(1, 4, 4, 1, true, true, false, false, false)));
	}

	pass P1
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(1, 4, 4, 0, true, true, false, false, false)));
	}
}

technique11 SkinnedTex2
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 1, true, true, false, false, false)));
	}

	pass P1
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 0, true, true, false, false, false))); 
	}
}

technique11 SkinnedTex3
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, SkinnedVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 1, true, true, false, false, true)));
	}

	pass P1
	{
		SetVS(CompileShader(vs_5_0, TexVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 0, true, true, false, false, false)));
	}
}

/////////////////////////////////////////////////////////////////////
// NormalMap Tech
/////////////////////////////////////////////////////////////////////
technique11 NormalMapLight1
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, NormalMapVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(1, 4, 4, 2, true, true, true, false, false)));
	}
}

technique11 NormalMapLight2
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, NormalMapVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 2, true, true, true, false, false)));
	}
}

technique11 NormalMapLight3
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, NormalMapVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 2, true, true, true, false, true)));
	}
}

technique11 ReflectionBasicLight
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 0, true, false, false, true, true)));
	}
};

technique11 ReflectionNormalMapLight
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, NormalMapVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 2, true, true, true, true, true)));
	}
};

/////////////////////////////////////////////////////////////////////
// Reflection Tech
/////////////////////////////////////////////////////////////////////
technique11 ReflectionLight1
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(1, 4, 4, 0, true, false, false, true, false)));
	}
}

technique11 ReflectionLight2
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(2, 4, 4, 0, true, false, false, true, false)));
	}
}

technique11 ReflectionLight3
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, BasicVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, PS(3, 4, 4, 0, true, false, false, true, true)));
	}
}

/////////////////////////////////////////////////////////////////////
// CubeMap Tech
/////////////////////////////////////////////////////////////////////
technique11 CubeMapSky
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, PosVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, CubeMapPS()));

		SetRasterizerState(NoCull);
		SetDepthStencilState(LessEqualDSS, 0);
	}
}

/////////////////////////////////////////////////////////////////////
// Shadow Tech
/////////////////////////////////////////////////////////////////////
technique11 ShadowMapNormal
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, ShadowNoramlVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, ShadowMapPS()));
		
		SetRasterizerState(Depth);
	}
}

technique11 ShadowMapSkin
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, ShadowSkinVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, ShadowMapPS()));

		SetRasterizerState(Depth);
	}
}

technique11 ShadowDebug
{
	pass P0
	{
		SetVS(CompileShader(vs_5_0, ShadowDebugVS()));
		SetGeometryShader(NULL);
		SetPS(CompileShader(ps_5_0, ShadowDebugPS()));
	}
};