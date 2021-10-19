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

struct BasicVertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
};

struct BasicVertexOut
{
    float4 PosH			 : SV_POSITION;
    float3 PosW			 : POSITIONW;
    float3 PosV			 : POSITIONV;
	float3 NormalW		 : NORMALW;
    float3 NormalV		 : NORMALV;
	float3 ViewDirection : VIEWDIR;

	float4 ShadowPosH	 : POS_SHADOW;
};

BasicVertexOut main(BasicVertexIn vin)
{
	BasicVertexOut vout;
	
	// ���� ���� ��ȯ
	vout.PosW = mul(gWorld, float4(vin.PosL, 1.0f)).xyz;

    vout.PosV = mul(gWorldView, float4(vin.PosL, 1.0f)).xyz;
	
	vout.NormalW = mul((float3x3)gWorldInvTranspose, vin.NormalL);
	vout.NormalW = normalize(vout.NormalW);
	
    vout.NormalV = mul((float3x3) gWorldInvTransposeView, vin.NormalL);
    vout.NormalV = normalize(vout.NormalV);
	
	//���� �������� ��ȯ
	vout.PosH = mul(gWorldViewProj, float4(vin.PosL, 1.0f));
	
	vout.ViewDirection = gEyePosW - vout.PosW;
	vout.ViewDirection = normalize(vout.ViewDirection);
	
	vout.ShadowPosH = mul(gShadowTransform, float4(vin.PosL, 1.0f));
	
	return vout;
}