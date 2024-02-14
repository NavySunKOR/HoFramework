#include "../Cores/Core.hlsli"

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);

    float2 uv;

    float theta = atan2(input.ModelPosition.z, input.ModelPosition.x);
    float phi = acos(input.ModelPosition.y / 3.f);

    uv.x = theta / 6.283184 + 0.5;
    uv.y = phi / 3.141592 ; 

    float4 textureColor = g_texture0.Sample(g_sampler, uv) ;

    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal , UsingMat),  1.f);
    return LightColor * textureColor;

}