cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld : packoffset(c0);
    float4x4 gWorldInvTranspose : packoffset(c4);
    float4x4 gTexTransform : packoffset(c12);
};

struct TexVertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct TexVertexOut
{
    float4 PosW : SV_POSITION;
    float3 NormalW : NORMALW;
    float2 Tex : TEXCOORD;
};

TexVertexOut main(TexVertexIn vin)
{
    TexVertexOut vout;

	// 세계 공간 변환
    vout.PosW = mul(gWorld, float4(vin.PosL, 1.0f));

    vout.NormalW = mul((float3x3) gWorldInvTranspose, vin.NormalL);
    vout.NormalW = normalize(vout.NormalW);
	
	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

    return vout;
}