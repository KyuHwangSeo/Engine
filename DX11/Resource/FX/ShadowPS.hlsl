#include "PixelHeader.hlsli"

struct ShadowVertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

float4 main(ShadowVertexOut pin) : SV_TARGET
{
	float4 diffuse = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
	clip(diffuse.a - 0.5f);

	return diffuse;
}