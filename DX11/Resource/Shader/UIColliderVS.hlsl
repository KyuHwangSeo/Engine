cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld : packoffset(c0);
    float4x4 gWorldInvTranspose : packoffset(c4);
    float4x4 gWorldViewProj : packoffset(c8);
    float4x4 gTexTransform : packoffset(c12);
    float4x4 gWorldView : packoffset(c16);
    float4x4 gWorldInvTransposeView : packoffset(c20);
};

struct UIVertexIn
{
    float3 PosL : POSITION;
    float4 Color : COLOR;
};

struct UIVertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

UIVertexOut main(UIVertexIn vin)
{
    UIVertexOut vout;
    
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f)).xyww;
    vout.Color = vin.Color;
    
    return vout;
}