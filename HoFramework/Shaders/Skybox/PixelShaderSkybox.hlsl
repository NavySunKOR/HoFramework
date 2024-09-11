#include "../Cores/SkyboxRenderPSCore.hlsli"


TextureCube g_texture0 : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    //float4 textureColor = float4(1.f,0.25f,0.25f,0.25f);
    float4 textureColor = g_texture0.Sample(g_sampler, input.ModelPosition.xyz);
    return textureColor;

}