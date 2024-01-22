#include "Lights.hlsli"

Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler : register(s0);

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float Dummy; // 4byte
    Material UsingMat; //48Byte
    
};

struct PSInput
{
    float4 ScreenPosition : SV_POSITION;
    float3 WorldPosition : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = (input.TexCoord.x < 0.3f) ? g_texture0.Sample(g_sampler, input.TexCoord) : g_texture1.Sample(g_sampler, input.TexCoord + float2(0.5, 0));
    
    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal, UsingMat), 1.f);
    
    
    return LightColor * textureColor;

}