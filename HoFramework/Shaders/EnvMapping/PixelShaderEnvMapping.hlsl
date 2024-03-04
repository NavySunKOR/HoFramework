#include "../Cores/Core.hlsli"

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};

TextureCube g_diffuseSkybox : register(t0);
TextureCube g_reflectionSkybox : register(t1);
SamplerState g_sampler : register(s0);


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = float4(1,1,1,1);


    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal , UsingMat),  1.f);


    float3 reflectVector = reflect(-toViewDirection, input.Normal);
    //LightColor *= g_textureSkybox.Sample(g_sampler, reflectVector);

    //IBL사용
    float4 Diffuse = g_diffuseSkybox.Sample(g_sampler, input.Normal) * 0.1f;
    float4 Specular = g_reflectionSkybox.Sample(g_sampler, reflectVector) * 5.f;

    float3 GlassFresnel = float3(0.02, 0.02, 0.02);
    Specular *= float4(ComputeSchlickFresnel(GlassFresnel, input.Normal, -toViewDirection), 1.f);

    LightColor = Diffuse + Specular;    //IBL사용으로 인한 덮어씌우기

    return LightColor * textureColor;

}