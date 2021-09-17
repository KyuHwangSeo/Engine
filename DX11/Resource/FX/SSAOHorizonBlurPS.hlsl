cbuffer cbTexel : register(b0)
{
	float gTexelSize;
};

cbuffer cbBlur : register(b1)
{
    float gWeights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
    };

    int gBlurRadius = 5;
}

Texture2D gNormalDepthMap	: register(t0);
Texture2D DepthSRV			: register(t1);

Texture2D gInputMap			: register(t2);

SamplerState samClampMinLinearPoint : register(s0);

struct VertexIn
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

float4 main(VertexIn pin) : SV_Target
{
	float2 texOffset = float2(gTexelSize, 0.0f);

    float gWeights[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
    };

    int gBlurRadius = 5;
	
	// The center value always contributes to the sum.
	float4 color = gWeights[5] * gInputMap.SampleLevel(samClampMinLinearPoint, pin.Tex, 0.0);
	float totalWeight = gWeights[5];

    float4 centerNormalDepth = float4(gNormalDepthMap.SampleLevel(samClampMinLinearPoint, pin.Tex, 0.0f).xyz, DepthSRV.SampleLevel(samClampMinLinearPoint, pin.Tex, 0.0f).z);

	for (float i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		// We already added in the center weight.
		if (i == 0)
			continue;

		float2 tex = pin.Tex + i * texOffset;

        float4 neighborNormalDepth = float4(
		gNormalDepthMap.SampleLevel(samClampMinLinearPoint, tex, 0.0f).xyz, 
		DepthSRV.SampleLevel(samClampMinLinearPoint, tex, 0.0f).z);

		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//

		if (dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
			abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f)
		{
			float weight = gWeights[i + gBlurRadius];

			// Add neighbor pixel to blur.
			color += weight * gInputMap.SampleLevel(
				samClampMinLinearPoint, tex, 0.0);

			totalWeight += weight;
		}
	}

	// Compensate for discarded samples by making total weights sum to 1.
	return color / totalWeight;
}