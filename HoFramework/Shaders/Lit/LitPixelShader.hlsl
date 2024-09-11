#include "../Cores/MeshRenderPSCore.hlsli"



float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = float4(1.f, 1.f, 1.f, 1.f);
    
    if (Mat.useAlbedoMap)
        textureColor = g_textureAlbedo.Sample(g_sampler, input.TexCoord);
    else
        textureColor = Mat.customAlbedo; //TODO : 나중에 Custom base color 넣기.
    
    if(Mat.useNormalMap)
    {
        float3 textureNormal = g_textureNormal.Sample(g_sampler, input.TexCoord);
        input.Normal = RecalculateNormal(textureNormal, input.Normal, input.Tangent);
    }
    
    
    float4 FinalColor = float4(0.f, 0.f, 0.f, 1);
    
    if (Mat.usePBR)
    {
        float roughness = (Mat.useRoughnessMap) ? g_textureRoughness.Sample(g_sampler, input.TexCoord).r : Mat.roughness;
        float metalic = (Mat.useMetallicMap) ? g_textureMetallic.Sample(g_sampler, input.TexCoord).r : Mat.metalic;
        
        float3 ambientColor = Mat.ambientStrength * textureColor.rgb;
        float3 lightVec = Lights[1].LightPos - input.WorldPosition;
        float3 radiance = GetFallOffAttenutation(length(lightVec), Lights[1].FalloffStart, Lights[1].FalloffEnd) * Lights[1].LightColor * Lights[1].LightIntensity;
        //float3 radiance = 1.f / (length(lightVec) * length(lightVec)) *Lights[1].LightColor;
        FinalColor.rgb = ambientColor + Shading::PBR::PBR(normalize(lightVec), toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        //FinalColor = clamp(FinalColor, 0, 1000.f);
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
    
    return FinalColor;

}