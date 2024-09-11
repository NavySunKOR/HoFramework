#include "../Cores/Shading.hlsli"
#ifndef LIGHTING
#define LIGHTING


namespace Lighting
{
    
    float3 ComputeDirectionalLightPhongModel(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
    {
        float3 LightVec = -pLight.LightDir;
        float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f);
 
        float Ambient = pMat.ambientStrength * AppliedIntensity;
        float Diffuse = max(dot(pNormalVector, LightVec), 0.0) * AppliedIntensity;
        float Specular = Shading::Phong::BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
        return Shading::Phong::PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
    }

    float3 ComputePointLightPhongModel(Light pLight, float3 pObjectPos, float3 pToViewDirection, float3 pNormalVector, Material pMat)
    {
        float3 LightVec = pLight.LightPos - pObjectPos;
        float D = length(LightVec);
    
        if (D > pLight.FalloffEnd)
        {
            return float3(0, 0, 0);
        }
    
        float Attenutation = GetFallOffAttenutation(D, pLight.FalloffStart, pLight.FalloffEnd);
    
        float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f) * Attenutation;
    
    
        float Ambient = pMat.ambientStrength * AppliedIntensity;
        float Diffuse = max(dot(pNormalVector, LightVec), 0.0) * AppliedIntensity;
        float Specular = Shading::Phong::BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
    
        return Shading::Phong::PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
    }

    float3 ComputeSpotLightPhongModel(Light pLight, float3 pObjectPos, float3 pToViewDirection, float3 pNormalVector, Material pMat)
    {
        float3 LightVec = pLight.LightPos - pObjectPos;
        float D = length(LightVec);
    
        if (D > pLight.FalloffEnd)
        {
            return float3(0, 0, 0);
        }
    
        LightVec /= D;
    
        float Attenutation = GetFallOffAttenutation(D, pLight.FalloffStart, pLight.FalloffEnd);
        float SpotFactor = pow(max(dot(-LightVec, pLight.LightDir), 0.f), pLight.SpotFactor);
    
        float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f) * Attenutation * SpotFactor;
    
        float Ambient = pMat.ambientStrength * AppliedIntensity;
        float Diffuse = max(dot(pNormalVector, LightVec), 0.0) * AppliedIntensity;
        float Specular = Shading::Phong::BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
        return Shading::Phong::PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
    }
}

#endif