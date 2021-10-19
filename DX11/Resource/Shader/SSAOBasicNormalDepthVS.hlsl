cbuffer cbSsaobject : register(b0)
{
	float4x4 gWorldView				: packoffset(c0);
	float4x4 gWorldInvTransposeView : packoffset(c4);
	float4x4 gWorldViewProj			: packoffset(c8);
	float4x4 gTexTransform			: packoffset(c12);
};

struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
};

struct VertexOut
{
	float4 PosH       : SV_POSITION;
	float3 PosV       : POSITION;
	float3 NormalV    : NORMAL;
	float2 Tex        : TEXCOORD0;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	// Transform to view space.
	vout.PosV = mul(gWorldView, float4(vin.PosL, 1.0f)).xyz;
	
	vout.NormalV = mul((float3x3)gWorldInvTransposeView, vin.NormalL);
    vout.NormalV = normalize(vin.NormalL);

	// Transform to homogeneous clip space.
	vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(gTexTransform, float4(0.0f, 0.0f, 0.0f, 1.0f)).xy;

	return vout;
}