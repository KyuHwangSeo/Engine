Texture2D gMask1 : register(t0);
Texture2D gMask2 : register(t1);
Texture2D gMask3 : register(t2);
Texture2D gMask4 : register(t3);

Texture2D gColor1 : register(t4);
Texture2D gColor2 : register(t4);
Texture2D gColor3 : register(t4);
Texture2D gColor4 : register(t4);

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
    if (tex1.rgb > 0.0f)
    {
        tex1.rgb = gColor1.Sample(samWrapMinLinear, pin.Tex) * tex1.a;
    }
    float4 tex2 = gMask2.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex2.rgb > 0.0f)
    {
        tex2.rgb = gColor1.Sample(samWrapMinLinear, pin.Tex) * tex2.a;
    }
    float4 tex3 = gMask3.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex3.rgb > 0.0f)
    {
        tex3.rgb = gColor1.Sample(samWrapMinLinear, pin.Tex) * tex3.a;
    }
    float4 tex4 = gMask4.Sample(samWrapMinLinear, pin.TerrainTex);
    if (tex4.rgb > 0.0f)
    {
        tex3.rgb = gColor1.Sample(samWrapMinLinear, pin.Tex) * tex4.a;
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