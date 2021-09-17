cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld					: packoffset(c0);
	float4x4 gWorldInvTranspose		: packoffset(c4);
	float4x4 gWorldViewProj			: packoffset(c8);
	float4x4 gTexTransform			: packoffset(c12);
	float4x4 gWorldView				: packoffset(c16);
    float4x4 gWorldInvTransposeView : packoffset(c20);
};

struct SkyCubeVertexIn
{
	float3 PosL  : POSITION;
};

struct SkyCubeVertexOut
{
	float4 PosH  : SV_POSITION;
	float3 PosL : POSITION;
};

SkyCubeVertexOut main(SkyCubeVertexIn vin)
{
	SkyCubeVertexOut vout;

	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f)).xyww;
	
	// Use local vertex position as cubemap lookup vector.
	vout.PosL = vin.PosL;
	
	return vout;
}