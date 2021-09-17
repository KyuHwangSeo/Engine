#include "PixelHeader.hlsli"

struct FinalVertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

float4 main(FinalVertexOut pin) : SV_TARGET
{
	return gDiffuseMap.Sample(samWrapAnisotropic, pin.Tex);
}