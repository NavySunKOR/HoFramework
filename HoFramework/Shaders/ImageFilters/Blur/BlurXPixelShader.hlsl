
#include"../Base/ImageFilterHeader.hlsli"


cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    float threshold;
    float strength;
    float4 options;
};

float4 main(SamplingPixelShaderInput input) : SV_TARGET{
    // Compute Shader X
    float3 color = float3(0, 0, 0);
    for (int i = 0; i < 5; ++i) {
        color += gaussianBlurWeights[i] * g_texture0.Sample(g_sampler,input.texcoord + float2(dx, 0) * float(i - 2)).rgb;
    }

    return float4(color,1.f);
}