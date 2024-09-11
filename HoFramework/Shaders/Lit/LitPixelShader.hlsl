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
    
    
    float4 FinalColor = float4(0, 0, 0, 1);
    
    if (Mat.usePBR)
    {
        float roughness = (Mat.useRoughnessMap) ? g_textureRoughness.Sample(g_sampler, input.TexCoord) : float3(Mat.roughness, Mat.roughness, Mat.roughness);
        float metalic = (Mat.useMetallicMap) ? g_textureMetallic.Sample(g_sampler, input.TexCoord) : Mat.metalic;
        
        int i = 0;
        
        {
            float3 lightVec = Lights[1].LightPos - input.WorldPosition;
            float3 radiance = Lights[1].LightIntensity * saturate(GetFallOffAttenutation(length(lightVec), Lights[1].FalloffStart, Lights[1].FalloffEnd)) * Lights[1].LightColor;
            FinalColor.rgb += Shading::PBR::PBR(lightVec, toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        }
        //[unroll]
        //for (i = 1; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
        //{
        //    float3 lightVec = Lights[i].LightPos - input.WorldPosition;
        //    float3 radiance = Lights[i].LightIntensity * saturate(GetFallOffAttenutation(length(lightVec), Lights[0].FalloffStart, Lights[0].FalloffEnd));

        //    FinalColor.rgb += PBR(lightVec, toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        //}
    }
    else //PBR이면
    { 
        float4 LightColor = (0, 0, 0, 1);
        LightColor += float4(Lighting::ComputeDirectionalLightPhongModel(Lights[0], toViewDirection, input.Normal, Mat), 1.f);
    
        int i = 1;
        [unroll]
        for (i = 1; i < 1 + NUM_POINT_LIGHT; ++i)
        {
            LightColor += float4(Lighting::ComputePointLightPhongModel(Lights[i], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
        }
    
    
        [unroll]
        for (i = 1 + NUM_POINT_LIGHT; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
        {
            LightColor += float4(Lighting::ComputeSpotLightPhongModel(Lights[i], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
        }
    

        if (Mat.useIBL)
        {
            float3 reflection = reflect(toViewDirection, input.Normal);
            LightColor += (SkyboxDiffuse.Sample(g_sampler, input.Normal) + SkyboxSpecular.Sample(g_sampler, normalize(reflection))) * Mat.roughness;
        }
    
        FinalColor = LightColor * textureColor;
    }

    FinalColor = LinearToneMapping(FinalColor, exposure, gamma);
    return FinalColor;

}