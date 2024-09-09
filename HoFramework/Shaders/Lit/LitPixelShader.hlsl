#include "../Cores/MeshRenderPSCore.hlsli"



float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = float4(1.f, 1.f, 1.f, 1.f);
    
    if (Mat.useAlbedoMap)
        textureColor = g_textureAlbedo.Sample(g_sampler, input.TexCoord);
    else
        textureColor = float4(1.f,1.f,1.f, 1.f); //TODO : 나중에 Custom base color 넣기.
    
    if(Mat.useNormalMap)
    {
        float3 textureNormal = g_textureNormal.Sample(g_sampler, input.TexCoord);
        input.Normal = RecalculateNormal(textureNormal, input.Normal, input.Tangent);
    }
    
    
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
    

    if (Mat.useIBL)
    {
        float3 reflection = reflect(toViewDirection, input.Normal);
        LightColor += (SkyboxDiffuse.Sample(g_sampler, input.Normal) + SkyboxSpecular.Sample(g_sampler, normalize(reflection))) * Mat.roughness;
    }
    
    float4 Color = LightColor * textureColor;
    Color = LinearToneMapping(Color, exposure, gamma);
    return Color;

}