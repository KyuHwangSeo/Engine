#include "PixelHeader.hlsli"

struct UIVertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(UIVertexOut pin) : SV_TARGET
{
    float4 texColor = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
    return texColor;
}