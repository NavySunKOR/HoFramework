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
    float3 halfWay = normalize(pToViewDirection + pLightDir);
    float hdotN = dot(halfWay, pNormalVector);
    float specular = pMat.specular * pow(max(hdotN, 0.f), pMat.shiness);
    
    return pMat.ambient + (pMat.diffuse + specular) * pLightIntensity;
}

float3 ComputeDirectionalLight(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 LightVec = -pLight.LightDir;
    float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f);
    return BlinnPhongModel(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);

}




