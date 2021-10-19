cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld					: packoffset(c0);
	float4x4 gWorldInvTranspose		: packoffset(c4);
	float4x4 gWorldViewProj			: packoffset(c8);
	float4x4 gTexTransform			: packoffset(c12);
	float4x4 gWorldView				: packoffset(c16);
    float4x4 gWorldInvTransposeView : packoffset(c20);
};

struct FinalVertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
};

struct FinalVertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

FinalVertexOut main(FinalVertexIn vin)
{
	FinalVertexOut vout;
	
	//동차 공간으로 변환
	vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

	// Output vertex attributes for interpolation across triangle.
	//vout.Tex = vin.Tex;
	vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

	return vout;
}