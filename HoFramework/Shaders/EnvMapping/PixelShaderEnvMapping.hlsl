#include "../Cores/Core.hlsli"

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};

TextureCube g_textureSkybox : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = float4(1,1,1,1);


    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal , UsingMat),  1.f);


    float3 reflectVector = reflect(-toViewDirection, input.Normal);
    LightColor *= g_textureSkybox.Sample(g_sampler, reflectVector);

    return LightColor * textureColor;

}