cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld					: packoffset(c0);
	float4x4 gWorldInvTranspose		: packoffset(c4);
	float4x4 gWorldViewProj			: packoffset(c8);
	float4x4 gTexTransform			: packoffset(c12);
	float4x4 gWorldView				: packoffset(c16);
    float4x4 gWorldInvTransposeView : packoffset(c20);
};

cbuffer cbSkinned : register(b1)
{
	float4x4 gBoneTransforms[96];
};

struct SkinVertexIn
{
	float3 PosL    : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex     : TEXCOORD;
    uint4 BoneIndices1 : BONEINDICES1;
    uint4 BoneIndices2 : BONEINDICES2;
    uint2 BoneIndices3 : BONEINDICES3;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
    float2 BoneWeights3 : WEIGHTS3;
};

struct ShadowVertexOut
{
	float4 PosH    : SV_POSITION;
	float2 Tex     : TEXCOORD;
};

ShadowVertexOut main(SkinVertexIn vin)
{
	ShadowVertexOut vout;

	float3 posL = float3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < 4; ++i)
	{
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    for (int k = 0; k < 2; ++k)
    {
        posL += vin.BoneWeights3[k] * mul(gBoneTransforms[vin.BoneIndices3[k]], float4(vin.PosL, 1.0f)).xyz;
    }
	
    vout.PosH = mul(gWorldViewProj, float4(posL, 1.0f));

	vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

	return vout;
}
