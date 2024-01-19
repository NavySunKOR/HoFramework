#include "Core.hlsli"

//32byte
struct Light
{
	//16
    float3 LightPos; //12
    float LightIntensity; //4 

	//16
    float3 LightDir; //12
    float Dummy; //4 
};

float3 BlinnPhongModel(float3 pLightDir,float3 pToViewDirection, float3 pNormalVector, float pLightIntensity , Material pMat)
{
    float halfWay = (pToViewDirection + pLightDir) / 2.f;
    float hdotN = dot(halfWay, pNormalVector);
    
    return pMat.ambient + (pMat.diffuse * pMat.specular) * pLightIntensity;
    
}

float3 ComputeDirectionalLight(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 AppliedLightDirection = -pLight.LightDir;
    float AppliedIntensity = pLight.LightIntensity * dot(AppliedLightDirection, pNormalVector);
    return BlinnPhongModel(AppliedLightDirection, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
}




