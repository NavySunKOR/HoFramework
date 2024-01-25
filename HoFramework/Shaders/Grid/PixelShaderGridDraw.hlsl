#include "../Cores/Core.hlsli"


cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    
    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal, UsingMat), 1.f);
    
    
    float4 TextureColor = g_texture.Sample(g_sampler, input.TexCoord);
    
    return LightColor * TextureColor;

}