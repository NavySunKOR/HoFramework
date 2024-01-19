#include "Lights.hlsli"

Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler : register(s0);

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 toViewDirection; //12byte
    Material UsingMat; //48Byte
    
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
    float3 Normal : WORLD;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 textureColor = (input.TexCoord.x < 0.5f) ? g_texture0.Sample(g_sampler, input.TexCoord) : g_texture1.Sample(g_sampler, input.TexCoord + float2(0.5, 0));
    
    Material NewMat = UsingMat;
    NewMat.diffuse = textureColor.xyz;
    NewMat.ambient = textureColor.xyz;
    
    float4 FinalColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal, NewMat), 1.f);
    return FinalColor;

}