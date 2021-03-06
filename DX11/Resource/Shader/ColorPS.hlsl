struct ColorVertexIn
{
	float4 PosH  : SV_POSITION;
	float4 Color : COLOR;
};

struct PixelOut
{
    float4 Albedo       : SV_Target0;
    float4 Normal       : SV_Target1;
    float4 Position     : SV_Target2;
    float4 Shadow       : SV_Target3;
    float4 NormalDepth  : SV_Target4;
};

PixelOut main(ColorVertexIn pin)
{
	PixelOut vout;

	vout.Albedo = pin.Color;
	vout.Normal = float4(0.0f, 0.0f, 0.0f, 0.0f);
	vout.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
    vout.Shadow = float4(0.0f, 0.0f, 0.0f, 1.0f);
    vout.NormalDepth = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return vout;
}