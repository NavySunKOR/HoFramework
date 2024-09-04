#include "../Cores/MeshRenderPSCore.hlsli"

Texture2D g_texture0 : register(t0);
TextureCube SkyboxDiffuse : register(t1);
TextureCube SkyboxSpecular : register(t2);
SamplerState g_sampler : register(s0);

static const int NUM_POINT_LIGHT = 1;
static const int NUM_SPOTLIGHT = 1;


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = g_texture0.Sample(g_sampler, input.TexCoord);

    
    float4 LightColor = float4(0, 0, 0, 1);
    
    LightColor += float4(ComputeDirectionalLight(Lights[0], toViewDirection, input.Normal, Mat), 1.f);
    
    int i = 1;
    [unroll]
    for (i = 1; i < 1 + NUM_POINT_LIGHT; ++i)
    {
        LightColor += float4(ComputePointLight(Lights[1], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
    }
    
    
    [unroll]
    for (i = 1 + NUM_POINT_LIGHT; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
    {
        LightColor += float4(ComputeSpotLight(Lights[2], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
    }
    
   
    float3 reflection = reflect(toViewDirection, input.Normal);

    LightColor += (SkyboxDiffuse.Sample(g_sampler, input.Normal) + SkyboxSpecular.Sample(g_sampler, normalize(reflection))) * Mat.roughness;
    

    return LightColor * textureColor;

}