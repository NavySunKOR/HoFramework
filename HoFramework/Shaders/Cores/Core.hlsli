

//32byte
struct Light
{
    float3 LightPos ; 
    float LightIntensity;
    float3 LightDir ; 
    float FalloffStart;
    float FalloffEnd; 
    float SpotFactor; 
    
    float Dummy1;
    float Dummy2;
};


struct Material
{
    float3 diffuse;
    float shiness;
    float3 ambient;
    float3 specular;
    
    float Dummy1;
    float Dummy2;
};



//Falloff�� 0~1���̷� ����
float GetFallOffAttenutation(float CurrentValue, float InFalloffStart, float InFalloffEnd)
{
    return saturate((InFalloffEnd - CurrentValue) / (InFalloffEnd - InFalloffStart));
}

float3 BlinnPhongModel(float3 pLightDir, float3 pToViewDirection, float3 pNormalVector, float pLightIntensity, Material pMat)
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

float3 ComputePointLight(Light pLight, float3 pObjectPos,float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 LightVec = pLight.LightPos - pObjectPos;
    float D = length(LightVec);
    
    if (D > pLight.FalloffEnd)
    {
        return float3(0, 0, 0);
    }
    
    float Attenutation = GetFallOffAttenutation(D, pLight.FalloffStart, pLight.FalloffEnd);
    
    float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f) * Attenutation;
    return BlinnPhongModel(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
}

float3 ComputeSpotLight(Light pLight, float3 pObjectPos, float3 pToViewDirection, float3 pNormalVector, Material pMat)
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
    return BlinnPhongModel(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
}

// Schlick approximation: Eq. 9.17 in "Real-Time Rendering 4th Ed."
// fresnelR0�� ������ ���� ����
// Water : (0.02, 0.02, 0.02)
// Glass : (0.08, 0.08, 0.08)
// Plastic : (0.05, 0.05, 0.05)
// Gold: (1.0, 0.71, 0.29)
// Silver: (0.95, 0.93, 0.88)
// Copper: (0.95, 0.64, 0.54)
// http://psgraphics.blogspot.com/2020/03/fresnel-equations-schlick-approximation.html
float3 ComputeSchlickFresnel(float3 fresnel0, float3 normal, float3 toEye)
{
    float Dot = saturate(dot(normal, toEye));

    float f0 = 1.f - Dot;

    // 1.0 ���� ���� ���� ���� �� ���ϸ� �� ���� ���� �˴ϴ�.
// 0�� -> f0 = 0 -> fresnelR0 ��ȯ
// 90�� -> f0 = 1.0 -> float3(1.0) ��ȯ
// 0���� ����� �����ڸ��� Specular ����, 90���� ����� ������ ���� ����(fresnelR0)
    return fresnel0 + (1.f - fresnel0) * pow(f0, 5.0);
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct PSInput
{
    float4 ModelPosition : POSITION0;
    float4 ScreenPosition : SV_POSITION;
    float3 WorldPosition : POSITION1;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};
