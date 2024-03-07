
#include"./ImageFilterHeader.hlsli"

cbuffer SamplingPixelConstantData : register(b0)
{
    float dx;
    float dy;
    float threshold;
    float strength;
    float4 options;
};



float4 main(SamplingPixelShaderInput input) : SV_TARGET
{
    return g_texture0.Sample(g_sampler,input.texcoord);
    //return float4(0,0,1,1);
}