Texture2D gMask1 : register(t0);

Texture2D gColor1 : register(t1);
Texture2D gNormal1 : register(t2);
Texture2D gColor2 : register(t3);
Texture2D gNormal2 : register(t4);
Texture2D gColor3 : register(t5);
Texture2D gNormal3 : register(t6);

Texture2D gMask2 : register(t7);

Texture2D gColor4 : register(t8);
Texture2D gNormal4 : register(t9);

SamplerState samWrapMinLinear : register(s0);

cbuffer cbID : register(b0)
{
    int gMatID : packoffset(c0.x);
};

struct VertexIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ViewDirection : VIEWDIR;

    float3 TangentW : TANGENTW;
    float3x3 TBN : TANGENT;
    
    float4 ShadowPosH : POS_SHADOW;
    float4 MaskColor1 : MASK_COLOR1;
    float4 MaskColor2 : MASK_COLOR2;
};

struct PixelOut
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Shadow : SV_Target3;
    float4 NormalDepth : SV_Target4;
};

PixelOut main(VertexIn pin)
{
    PixelOut vout;
	
    float3 normalColor = float3(0.0f, 0.0f, 0.0f);
    float3 normalMapSample = float3(0.0f, 0.0f, 0.0f);
    
    float4 mask = float4(pin.MaskColor1.rgb, pin.MaskColor2.r);
    float length = mask.r + mask.g + mask.b + mask.a;
    mask /= length;
    
    
    float4 albedo = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    if (mask.r > 0.0f)
    {
        albedo.rgb += gColor1.Sample(samWrapMinLinear, pin.Tex).rgb * mask.r;
        normalColor = gNormal1.Sample(samWrapMinLinear, pin.Tex).rgb;
        normalMapSample += (2.0f * normalColor - 1.0f) * mask.r;
    }
    if (mask.g > 0.0f)
    {
        albedo.rgb += gColor2.Sample(samWrapMinLinear, pin.Tex).rgb * mask.g;
        normalColor = gNormal2.Sample(samWrapMinLinear, pin.Tex).rgb;
        normalMapSample += (2.0f * normalColor - 1.0f) * mask.g;
    }
    if (mask.b > 0.0f)
    {
        albedo.rgb += gColor3.Sample(samWrapMinLinear, pin.Tex).rgb * mask.b;
        normalColor = gNormal3.Sample(samWrapMinLinear, pin.Tex).rgb;
        normalMapSample += (2.0f * normalColor - 1.0f) * mask.b;
    }
    if (mask.a > 0.0f)
    {
        albedo.rgb += gColor4.Sample(samWrapMinLinear, pin.Tex).rgb * mask.a;
        normalColor = gNormal4.Sample(samWrapMinLinear, pin.Tex).rgb;
        normalMapSample += (2.0f * normalColor - 1.0f) * mask.a;
    }
    
    float3 bumpedNormalW = mul(normalMapSample, pin.TBN);

    vout.Albedo = albedo;
    vout.Normal = float4(bumpedNormalW, 1.0f);
    vout.Position = float4(pin.PosW, gMatID);
    vout.Shadow = float4(pin.ShadowPosH.xyz / pin.ShadowPosH.w, 0.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, pin.PosV.z);
    
    return vout;
}