Texture2D gDiffuseMap : register(t0);

SamplerState samWrapMinLinear : register(s0);

struct VertexIn
{
    float4 PosW : SV_POSITION;
    float3 NormalW : NORMALW;
    float2 Tex : TEXCOORD;
};

struct PixelOut
{
    float4 Color : SV_Target0;
};

PixelOut main(VertexIn pin)
{
    PixelOut pout;
    
    pout.Color = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
    
    return pout;
}