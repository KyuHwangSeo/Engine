#include "PixelHeader.hlsli"

struct UIVertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

float4 main(UIVertexOut pin) : SV_TARGET
{
    return pin.Color;
}