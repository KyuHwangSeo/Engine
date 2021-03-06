cbuffer cbPerObject : register(b0)
{
    float4x4 gWorld : packoffset(c0);
    float4x4 gWorldInvTranspose : packoffset(c4);
    float4x4 gWorldViewProj : packoffset(c8);
    float4x4 gTexTransform : packoffset(c12);
    float4x4 gWorldView : packoffset(c16);
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

struct TerrainVertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
    float4 Mask1 : MASK1;
    float4 Mask2 : MASK2;
};

struct TerrainVertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITIONW;
    float3 PosV : POSITIONV;
    float3 NormalW : NORMALW;
    float3 NormalV : NORMALV;
    float2 Tex : TEXCOORD;
    float3 ViewDirection : VIEWDIR;

    float3 TangentW : TANGENTW;
    float3x3 TBN : TANGENT;
    
    float4 ShadowPosH : POS_SHADOW;
    float4 MaskColor1 : MASK_COLOR1;
    float4 MaskColor2 : MASK_COLOR2;
};


TerrainVertexOut main(TerrainVertexIn vin)
{
    TerrainVertexOut vout;

	// 세계 공간 변환
    vout.PosW = mul(gWorld, float4(vin.PosL, 1.0f)).xyz;

    vout.PosV = mul(gWorldView, float4(vin.PosL, 1.0f)).xyz;
	
    vout.NormalW = mul((float3x3) gWorldInvTranspose, vin.NormalL);
    vout.NormalW = normalize(vout.NormalW);
	
    vout.NormalV = mul((float3x3) gWorldInvTransposeView, vin.NormalL);
    vout.NormalV = normalize(vout.NormalV);
	
	// 동차 공간으로 변환
    vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));

    vout.ViewDirection = gEyePosW - vout.PosW;
    vout.ViewDirection = normalize(vout.ViewDirection);

	// Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(gTexTransform, float4(vin.Tex, 0.0f, 1.0f)).xy;

    vout.ShadowPosH = mul(gShadowTransform, float4(vin.PosL, 1.0f));
    
    vout.TangentW = mul((float3x3) gWorld, vin.TangentL);

	// Vertex Shader 에서 TBN을 구해주자..
	// Pixel Shader에서 연산은 최소한으로 해야하기 때문..
    float3 N = vout.NormalW;
    float3 T = normalize(vout.TangentW - dot(vout.TangentW, N) * N);
    float3 B = cross(N, T);

    vout.TBN = float3x3(T, B, N);
    
    // Terrain Texture Index
    vout.MaskColor1 = vin.Mask1;
    vout.MaskColor2 = vin.Mask2;
	
    return vout;
}