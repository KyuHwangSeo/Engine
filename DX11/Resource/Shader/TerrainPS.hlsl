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
    bool gTex : packoffset(c0.y);
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

    float4 ShadowPosH : POS_SHADOW;
    float2 TerrainTex : TEXCOORD1;
};

struct PixelOut
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Light : SV_Target3;
    float4 Shadow : SV_Target4;
    float4 NormalDepth : SV_Target5;
    float4 Depth : SV_Target6;
};

PixelOut main(VertexIn pin) : SV_TARGET
{
    PixelOut vout;
	
    float4 mask1 = gMask1.Sample(samWrapMinLinear, pin.TerrainTex);
    float4 tex1 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (mask1.r > 0.0f)
    {
        tex1.rgb += gColor1.Sample(samWrapMinLinear, pin.Tex).rgb * mask1.r;
    }
    if (mask1.g > 0.0f)
    {
        tex1.rgb += gColor2.Sample(samWrapMinLinear, pin.Tex).rgb * mask1.g;
    }
    if (mask1.b > 0.0f)
    {
        tex1.rgb += gColor3.Sample(samWrapMinLinear, pin.Tex).rgb * mask1.b;
    }
    
    float4 mask2 = gMask2.Sample(samWrapMinLinear, pin.TerrainTex);
    float4 tex2 = float4(0.0f, 0.0f, 0.0f, 0.0f);
    if (mask2.r > 0.0f)
    {
        tex2.rgb += gColor4.Sample(samWrapMinLinear, pin.Tex).rgb * mask2.r;
    }
    
    float4 albedo = float4(float3(tex1.rgb + tex2.rgb), 1.0f);
    
    vout.Albedo = albedo;
    vout.Normal = float4(pin.NormalW, 1.0f);
    vout.Position = float4(pin.PosW, 1.0f);
    vout.Light = float4(0.0f, gMatID, 0.0f, 1.0f);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 1.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, 1.0f);
    vout.Depth = float4(pin.PosH.z / pin.PosH.w, pin.ShadowPosH.w, pin.PosV.z, 1.0f);
    
    return vout;
}