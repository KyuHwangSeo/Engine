Texture2D gMask1 : register(t0);
Texture2D gColor1 : register(t1);
Texture2D gNormal1 : register(t2);

Texture2D gMask2 : register(t3);
Texture2D gColor2 : register(t4);
Texture2D gNormal2 : register(t5);

Texture2D gMask3 : register(t6);
Texture2D gColor3 : register(t7);
Texture2D gNormal3 : register(t8);

Texture2D gMask4 : register(t9);
Texture2D gColor4 : register(t10);
Texture2D gNormal4 : register(t11);

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
	
    float4 tex1 = gMask1.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex1.r > 0.0f)
    {
        tex1.rgb = gColor1.Sample(samWrapMinLinear, pin.Tex) * tex1.r;
    }
    float4 tex2 = gMask2.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex2.r > 0.0f)
    {
        tex2.rgb = gColor2.Sample(samWrapMinLinear, pin.Tex) * tex2.r;
    }
    float4 tex3 = gMask3.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex3.r > 0.0f)
    {
        tex3.rgb = gColor3.Sample(samWrapMinLinear, pin.Tex) * tex3.r;
    }
    float4 tex4 = gMask4.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex4.r > 0.0f)
    {
        tex4.rgb = gColor4.Sample(samWrapMinLinear, pin.Tex) * tex4.r;
    }
    
    float4 albedo = float4(float3(tex1.rgb + tex2.rgb + tex3.rgb + tex4.rgb), 1.0f);
    
    vout.Albedo = albedo;
    vout.Normal = float4(pin.NormalW, 1.0f);
    vout.Position = float4(pin.PosW, 1.0f);
    vout.Light = float4(0.0f, gMatID, 0.0f, 1.0f);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 1.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, 1.0f);
    vout.Depth = float4(pin.PosH.z / pin.PosH.w, pin.ShadowPosH.w, pin.PosV.z, 1.0f);
    
    return vout;
}