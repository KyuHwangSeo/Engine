#include "PixelHeader.hlsli"

cbuffer cbID : register(b0)
{
    int gMatID : packoffset(c0.x);
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

    float3 TangentW		 : TANGENTW;
    float3x3 TBN		 : TANGENT;
	
	float4 ShadowPosH	 : POS_SHADOW;
};

struct PixelOut
{
    float4 Albedo       : SV_Target0;
    float4 Normal       : SV_Target1;
    float4 Position     : SV_Target2;
    float4 Shadow       : SV_Target3;
    float4 NormalDepth  : SV_Target4;
};

PixelOut main(VertexIn pin)
{
	PixelOut vout;

	float4 albedo = gDiffuseMap.Sample(samWrapMinLinear, pin.Tex);

	float3 normalMapSample = 2.0f * gNormalMap.Sample(samWrapMinLinear, pin.Tex).rgb - 1.0f;
	float3 bumpedNormalW = mul(normalMapSample, pin.TBN);
	
    vout.Albedo = albedo;
    vout.Normal = float4(bumpedNormalW, 1.0f);
    vout.Position = float4(pin.PosW, gMatID);
    vout.Shadow = float4(pin.ShadowPosH.xyz / pin.ShadowPosH.w, 0.0f);
    vout.NormalDepth = float4(pin.NormalV.xyz, pin.PosV.z);
    
	return vout;
}