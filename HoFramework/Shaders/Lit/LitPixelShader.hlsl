#include "../Cores/Core.hlsli"

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight[3]; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};

cbuffer ExtraPixelBuffer : register(b1)
{
    float RimIntensity;
    float RimPow;
    float Dummy1;
    float Dummy2;
};


Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

static const int NUM_POINT_LIGHT = 1;
static const int NUM_SPOTLIGHT = 1;


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = g_texture0.Sample(g_sampler, input.TexCoord);

    
    float4 LightColor = float4(0, 0, 0, 1);
    
    LightColor += float4(ComputeDirectionalLight(UsingLight[0], toViewDirection, input.Normal, UsingMat), 1.f);
    
    //int i = 1;
    //[unroll]
    //for (i = 1; i < 1 + NUM_POINT_LIGHT; ++i)
    //{
    //    LightColor += float4(ComputePointLight(UsingLight[i],input.WorldPosition, toViewDirection, input.Normal, UsingMat), 1.f);
    //}
    
    // [unroll]
    //for (i = 1 + NUM_POINT_LIGHT ; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
    //{
    //    LightColor += float4(ComputeSpotLight(UsingLight[i], input.WorldPosition, toViewDirection, input.Normal, UsingMat), 1.f);
    //}
    
    
    
    //��ó��
    //float Dot = dot(toViewDirection, input.Normal);
    //Dot = smoothstep(0.f, 1.f, 1.f - Dot);
    //Dot = pow(abs(Dot), RimPow);
    //LightColor += Dot * RimIntensity;

    return LightColor * textureColor;

}