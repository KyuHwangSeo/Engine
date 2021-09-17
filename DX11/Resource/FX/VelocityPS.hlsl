Texture2D gDepthMap : register(t0);

SamplerState samWrapMinLinear : register(s0);

cbuffer cbFullScreen : register(b0)
{
    float4x4 gNowViewProj : packoffset(c0);
    float4x4 gPrevViewProj : packoffset(c4);
    float gDeltaTime : packoffset(c8.x);
};

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(VertexIn pin) : SV_TARGET
{
    float4 zOverW = 1.0f;
    zOverW = gDepthMap.Sample(samWrapMinLinear, pin.Tex);
    
    float4 H = float4(pin.Tex.x * 2 - 1, pin.Tex.y * -2 + 1, zOverW.r, 1);
    
    float4 D = mul(gNowViewProj, H);
    
    float4 worldPos = D / D.w;
    worldPos.w = 1;
    
    float4 currentPos = H;
    
    float4 previousPos = mul(gPrevViewProj, worldPos);
    previousPos /= previousPos.w;

    float2 velocity = (currentPos.xy - previousPos.xy) / 2 * gDeltaTime;
    velocity.y *= -1;
    
    velocity.x = clamp(velocity.x, 0.001f, 0.003f);
    velocity.y = clamp(velocity.y, 0.001f, 0.003f);
    
    return float4(velocity, 0, 1);
}