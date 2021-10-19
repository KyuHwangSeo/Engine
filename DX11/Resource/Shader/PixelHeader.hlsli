#include "LightHelper.fx"

Texture2D gDiffuseMap	 : register(t0);
Texture2D gNormalMap	 : register(t1);
TextureCube gCubeMap	 : register(t2);

// °ø¿ë TextureMap
Texture2D gShadowMap     : register(t16);
Texture2D gSSAOMap       : register(t17);

SamplerComparisonState gShadowSam	: register(s0);
SamplerState samWrapMinLinear		: register(s1);
SamplerState samWrapAnisotropic		: register(s2);
SamplerState samClampMinLinear		: register(s3);