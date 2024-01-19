#include "Core.hlsli"

//30byte
struct Light
{
    float3 LightPos; //12
    float3 LightDir; //12
    float LightIntensity; //4 
    
    bool Dummy1; //1
    bool Dummy2; //1
};

float3 BlinnPhongModel(float3 pLightDir,float3 pToViewDirection, float3 pNormalVector, float pLightIntensity , Material pMat)
{
    pLightDir = -pLightDir;
    float halfWay = (pToViewDirection + pLightDir) / 2.f;
    float hdotN = dot(halfWay, pNormalVector);
    
    return pMat.ambient + (pMat.diffuse * pMat.specular) * pLightIntensity;
    
}

float3 ComputeDirectionalLight(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 AppliedLightDirection = pLight.LightDir;
    float AppliedIntensity = pLight.LightIntensity *  dot(pLight.LightDir, pNormalVector);
    return BlinnPhongModel(AppliedLightDirection,pToViewDirection,pNormalVector,AppliedIntensity,pMat);
}




