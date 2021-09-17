struct SkyCubeVertexIn
{
	float4 PosH  : SV_POSITION;
	float3 PosL : POSITION;
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

TextureCube gCubeMap	: register(t3);

SamplerState samWrapMinLinear : register(s3);

PixelOut main(SkyCubeVertexIn pin) : SV_TARGET
{
	PixelOut vout;

	vout.Albedo = gCubeMap.Sample(samWrapMinLinear, pin.PosL);
	vout.Normal = float4(0.0f, 0.0f, 0.0f, 1.0f);
	vout.Position = float4(0.0f, 0.0f, 0.0f, 1.0f);
	vout.Light = float4(1.0f, 0.0f, 0.0f, 1.0f);
    vout.Shadow = float4(0.0f, 0.0f, 0.0f, 1.0f);
    vout.NormalDepth = float4(0.0f, 0.0f, 0.0f, 1.0f);
    vout.Depth = float4(0.0f, 0.0f, 0.0f, 1.0f);

	return vout;
}