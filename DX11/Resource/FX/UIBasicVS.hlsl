cbuffer cbPerUI : register(b0)
{
    float4x4 gWorldViewProj;
}

struct UIVertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct UIVertexOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

UIVertexOut main(UIVertexIn vin)
{
    UIVertexOut vout;
    
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f)).xyww;
    vout.Tex = vin.Tex;
    
    return vout;
}
