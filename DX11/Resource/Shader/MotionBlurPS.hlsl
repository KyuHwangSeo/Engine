Texture2D gOrigin : register(t0);
Texture2D gVelocity : register(t1);

SamplerState samWrapMinLinear : register(s0);

cbuffer cbMotionBlur : register(b0)
{
    int gMotionBlurSampleCount = 30;
}

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(VertexIn pin) : SV_Target
{
    float4 Velocity = gVelocity.Sample(samWrapMinLinear, pin.Tex);
    
    float4 Out = gOrigin.Sample(samWrapMinLinear, pin.Tex);
    
    float4 BColor = 0;
    float2 NowTex = 0;
    for (int i = 0; i < gMotionBlurSampleCount; i++)
    {
        NowTex += Velocity.xy;
        BColor = gOrigin.Sample(samWrapMinLinear, pin.Tex + NowTex);
        Out += BColor;
    }
    
    Out /= (gMotionBlurSampleCount + 1);
      
    return float4(Out.xyz, 1.0f);
}