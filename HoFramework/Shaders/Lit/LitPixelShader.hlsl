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
    
    if (Mat.usePBR)//PBR이면
    {
        float roughness = (Mat.useRoughnessMap) ? g_textureRoughness.Sample(g_sampler, input.TexCoord).r : Mat.roughness;
        float metalic = (Mat.useMetallicMap) ? g_textureMetallic.Sample(g_sampler, input.TexCoord).r : Mat.metalic;
        float3 ambientColor = (Mat.useIBL)? IBLUsingBRDF(textureColor, input.Normal, toViewDirection, metalic, roughness) : Mat.ambientStrength * textureColor;
        
        FinalColor.rgb = ambientColor;
        
        //Directional Light
        //Half way vector 계산 때문에 역수를 취함.
        float3 lightVec = -Lights[0].LightDir;
        float3 radiance = Lights[0].LightColor * Lights[0].LightIntensity;
        FinalColor.rgb += Shading::PBR::PBR(normalize(lightVec), toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        
        
        
        int i = 1;
        [unroll]
        for (i = 1; i < 1 + NUM_POINT_LIGHT; ++i)
        {
            //Half way vector 계산 때문에 역수를 취함.
            float3 lightVec = -(input.WorldPosition - Lights[i].LightPos);
            float3 radiance = GetFallOffAttenutation(length(lightVec), Lights[i].FalloffStart, Lights[i].FalloffEnd) * Lights[i].LightColor * Lights[i].LightIntensity;
            FinalColor.rgb += Shading::PBR::PBR(normalize(lightVec), toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        }
        
        [unroll]
        for (i = 1 + NUM_POINT_LIGHT; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
        {
            //Half way vector 계산 때문에 역수를 취함.
            float3 lightVec = -(input.WorldPosition - Lights[i].LightPos);
            float spotFactor = pow(max(dot(-lightVec, Lights[i].LightDir), 0.f), Lights[i].SpotFactor);
            float3 radiance = GetFallOffAttenutation(length(lightVec), Lights[i].FalloffStart, Lights[i].FalloffEnd) * spotFactor * Lights[i].LightColor * Lights[i].LightIntensity;
            FinalColor.rgb += Shading::PBR::PBR(normalize(lightVec), toViewDirection, input.Normal, textureColor.rgb, metalic, roughness, radiance);
        }
        
    }
    else 
    { 
        float4 LightColor = float4(0, 0, 0, 1);
        
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
            LightColor += IBLUsingPhong(input.Normal, toViewDirection, Mat);
        }
    
        FinalColor = LightColor * textureColor;
    }
    
    return FinalColor;

}