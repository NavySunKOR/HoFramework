
#include"./ImageFilterHeader.hlsli"

cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    float threshold;
    float strength;
    
    float gamma;
    float exposure;
    float dummy1;
    float dummy2;
};



float4 main(SamplingPixelShaderInput input) : SV_TARGET
{
    float4 color = g_texture0.Sample(g_sampler, input.texcoord);
    color = LinearToneMapping(color, exposure, gamma);
    
    return color;
}