#include "PixelHeader.hlsli"

cbuffer cbID : register(b0)
{
    int gMatID : packoffset(c0.x);
    bool gTex  : packoffset(c0.y);
};

struct VertexIn
{
    float4 PosH			 : SV_POSITION;
    float3 PosW			 : POSITIONW;
    float3 PosV			 : POSITIONV;
	float3 NormalW		 : NORMALW;
    float3 NormalV		 : NORMALV;
	float2 Tex			 : TEXCOORD;
	float3 ViewDirection : VIEWDIR;

	float4 ShadowPosH	 : POS_SHADOW;
};

struct PixelOut
{
	float4 Albedo		: SV_Target0;
	float4 Normal		: SV_Target1;
	float4 Position		: SV_Target2;
	float4 Light		: SV_Target3;
    float4 Shadow		: SV_Target4;
    float4 NormalDepth	: SV_Target5;
    float4 Depth		: SV_Target6;
};

PixelOut main(VertexIn pin) : SV_TARGET
{
	PixelOut vout;
	
	float4 albedo = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);
	
	vout.Albedo = albedo;
	vout.Normal = float4(pin.NormalW, 1.0f);
	vout.Position = float4(pin.PosW, 1.0f);
    vout.Light = float4(0.0f, gMatID, 0.0f, 1.0f);
    vout.Shadow = float4(pin.ShadowPosH.xyz, 1.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, 1.0f);
    vout.Depth = float4(pin.PosH.z / pin.PosH.w, pin.ShadowPosH.w, pin.PosV.z, 1.0f);
    
	return vout;
}