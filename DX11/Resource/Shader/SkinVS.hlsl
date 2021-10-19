cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld					: packoffset(c0);
	float4x4 gWorldInvTranspose		: packoffset(c4);
	float4x4 gWorldViewProj			: packoffset(c8);
	float4x4 gTexTransform			: packoffset(c12);
	float4x4 gWorldView				: packoffset(c16);
    float4x4 gWorldInvTransposeView : packoffset(c20);
};

cbuffer cbCamera : register(b1)
{
	float3 gEyePosW : packoffset(c0);
};

cbuffer cbShadow : register(b2)
{
	float4x4 gShadowTransform : packoffset(c0);
};

cbuffer cbSkinned : register(b3)
{
	float4x4 gBoneTransforms[96];
};

struct SkinVertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    uint4 BoneIndices1 : BONEINDICES1;
    uint4 BoneIndices2 : BONEINDICES2;
    uint2 BoneIndices3 : BONEINDICES3;
    float4 BoneWeights1 : WEIGHTS1;
    float4 BoneWeights2 : WEIGHTS2;
    float2 BoneWeights3 : WEIGHTS3;
};

struct SkinVertexOut
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

SkinVertexOut main(SkinVertexIn vin)
{
	SkinVertexOut vout;

	float3 posL = float3(0.0f, 0.0f, 0.0f);
	float3 normalL = float3(0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < 4; ++i)
	{
        posL += vin.BoneWeights1[i] * mul(gBoneTransforms[vin.BoneIndices1[i]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights1[i] * mul((float3x3) gBoneTransforms[vin.BoneIndices1[i]], vin.NormalL);
    }

    for (int j = 0; j < 4; ++j)
    {
        posL += vin.BoneWeights2[j] * mul(gBoneTransforms[vin.BoneIndices2[j]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights2[j] * mul((float3x3) gBoneTransforms[vin.BoneIndices2[j]], vin.NormalL);
    }
	
    for (int k = 0; k < 2; ++k)
    {
        posL += vin.BoneWeights3[k] * mul(gBoneTransforms[vin.BoneIndices3[k]], float4(vin.PosL, 1.0f)).xyz;
        normalL += vin.BoneWeights3[k] * mul((float3x3) gBoneTransforms[vin.BoneIndices3[k]], vin.NormalL);
    }
	
	// 세계 공간 변환
	vout.PosW = mul(gWorld, float4(posL, 1.0f)).xyz;
    vout.PosV = mul(gWorldView, float4(posL, 1.0f)).xyz;
	
	vout.NormalW = mul((float3x3)gWorldInvTranspose, normalL);
	vout.NormalW = normalize(vout.NormalW);
	
    vout.NormalV = mul((float3x3) gWorldInvTransposeView, normalL);
    vout.NormalV = normalize(vout.NormalV);
	
	//동차 공간으로 변환
	vout.PosH = mul(gWorldViewProj, float4(posL, 1.0f));
	
	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);
	
	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;
	
	vout.ShadowPosH = mul(gShadowTransform, float4(posL, 1.0f));

	return vout;
}