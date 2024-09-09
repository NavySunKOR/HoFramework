

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


//TODO : albedo,metalic,fresnel0,shiness 추가 
//Shiness는 Roughness의 Inverse로도 대체할 수는 있었으나, 그러기엔 1 이상의 값이 필요 할 경우가 생겨서 아예 별도 추가.
struct Material
{
    float3 albedo;
    float roughness;
    
    float3 fresnelR0;
    float metalic;
    
    float3 specular;
    float shiness;
    
    bool useAlbedoMap; 
    bool useNormalMap; 
    bool useHeightMap; 
    bool useAmbientOcclusionMap;
    
    bool useMetallicMap; 
    bool useRoughnessMap;
    bool useIBL;
    
    uint shadingModel;
    
    float Dummy1;
    float Dummy2;
};


const uint SHADINGMODEL_PBR = 0;
const uint SHADINGMODEL_BLINNPHONG = 1;


//Falloff를 0~1사이로 리턴
float GetFallOffAttenutation(float CurrentValue, float InFalloffStart, float InFalloffEnd)
{
    return saturate((InFalloffEnd - CurrentValue) / (InFalloffEnd - InFalloffStart));
}

//
float3 BlinnPhongModel(float3 pLightDir, float3 pToViewDirection, float3 pNormalVector, float pLightIntensity, Material pMat)
{
    float3 halfWay = normalize(pToViewDirection + pLightDir);
    float hdotN = dot(halfWay, pNormalVector);
    float specular = pMat.specular * pow(max(hdotN, 0.f), pMat.roughness);
    
    return float3(0.1f, 0.1f, 0.1f) + 
    (pMat.albedo + specular * pLightIntensity);
}


//InRadiance는 포인트라이트 계열에만 동작함. 아닌경우 1,1,1 이다.
float4 PBR(float3 InLightVec, float3 InPixelToViewVector, Material inMat, float3 InRadiance)
{
    float4 DiffuseBRDF = float4(1.f, 1.f, 1.f, 1.f);
    float4 SpecularBRDF = float4(1.f, 1.f, 1.f, 1.f);
    
    //float3 halfway = normalize(pixelToEye + lightVec);
        
    //float NdotI = max(0.0, dot(normalWorld, lightVec));
    //float NdotH = max(0.0, dot(normalWorld, halfway));
    //float NdotO = max(0.0, dot(normalWorld, pixelToEye));
        
    //const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    //float3 F0 = lerp(Fdielectric, albedo, metallic);
    //float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, pixelToEye)));
    //float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metallic);
    //float3 diffuseBRDF = kd * albedo;

    //float D = NdfGGX(NdotH, roughness);
    //float3 G = SchlickGGX(NdotI, NdotO, roughness);
        
    //    // 방정식 (2), 0으로 나누기 방지
    //float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

    //float3 radiance = light[i].radiance * saturate((light[i].fallOffEnd - length(lightVec)) / (light[i].fallOffEnd - light[i].fallOffStart));
    
    return float4(1, 1, 1, 1);
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
// fresnelR0는 물질의 고유 성질
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

    // 1.0 보다 작은 값은 여러 번 곱하면 더 작은 값이 됩니다.
// 0도 -> f0 = 0 -> fresnelR0 반환
// 90도 -> f0 = 1.0 -> float3(1.0) 반환
// 0도에 가까운 가장자리는 Specular 색상, 90도에 가까운 안쪽은 고유 색상(fresnelR0)
    return fresnel0 + (1.f - fresnel0) * pow(f0, 5.0);
}


//InNormalMapValues <- 0~1 사이로 넣을것.
float3 RecalculateNormal(float3 InNormalMapValues, float3 InNormalWorld, float3 InTangentWorld)
{
    InNormalMapValues = InNormalMapValues * 2.f - 1;
    
    float3 N = InNormalWorld;
    float3 T = normalize(InTangentWorld - dot(InTangentWorld, N) * N); //Normal 쪽에 프로젝션한 값을 탄젠트에 빼준다.
    float3 B = cross(N, T);
    
    float3x3 TBN = float3x3(T, B, N);
    float3 newNormal = mul(InNormalMapValues, TBN);
    newNormal = normalize(newNormal);
    
    return newNormal;
}

float4 LinearToneMapping(float4 InColor,float InExposure, float InGamma)
{
    float4 invGamma = float4(1.f,1.f,1.f,1.f) / InGamma;
    InColor = clamp(InColor * InExposure, 0, 1) ;
    InColor = pow(InColor, invGamma);

    InColor.a = 1.f;
    return InColor;
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
};

struct PSInput
{
    float4 ModelPosition : POSITION0;
    float4 ScreenPosition : SV_POSITION;
    float3 WorldPosition : POSITION1;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT0;
};
