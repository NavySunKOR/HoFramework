

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
    
    float3 LightColor;
    float Dummy3;
};


//TODO : albedo,metalic,fresnel0,shiness 추가 
//Shiness는 Roughness의 Inverse로도 대체할 수는 있었으나, 그러기엔 1 이상의 값이 필요 할 경우가 생겨서 아예 별도 추가.
struct Material
{
    float ambientStrength;
    float roughness;
    float metalic;
    float shiness;

    float4 fresnelR0;
    
    float4 specular;
    
    bool useAlbedoMap;
    bool useNormalMap;
    bool useHeightMap;
    bool useAmbientOcclusionMap;
    
    bool useMetallicMap;
    bool useRoughnessMap;
    bool useIBL;
    bool usePBR;
};


//Falloff를 0~1사이로 리턴
float GetFallOffAttenutation(float CurrentValue, float InFalloffStart, float InFalloffEnd)
{
    return saturate((InFalloffEnd - CurrentValue) / (InFalloffEnd - InFalloffStart));
}

float3 SchlickFresnel(float3 F0, float NdotH)
{
    // TODO: 방정식 (5)
    float3 ResultFresnel = F0 + (1 - F0) * pow(2.f, (-5.55473 * NdotH - 6.98316) * NdotH);
    
    return ResultFresnel;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float NdfGGX(float NdotH, float roughness)
{
    // TODO: 방정식 (3)
    
    float a = roughness * roughness;
    float dh = (a * a) / (3.141592 * ((NdotH * NdotH) * (a * a - 1) + 1) * ((NdotH * NdotH) * (a * a - 1) + 1));
    
    return dh;
}

float SchlickGGX(float NdotI, float NdotO, float roughness)
{
    
    float k = ((roughness + 1) * (roughness + 1)) / 8;
    float g1l = NdotI / (NdotI * (1 - k) + k);
    float g1v = NdotO / (NdotO * (1 - k) + k);
    
    
    return g1l * g1v;
}

//
float BRDFSpecularBlinnPhong(float3 pLightDir, float3 pToViewDirection, float3 pNormalVector, float pLightIntensity, Material pMat)
{
    float3 halfWay = normalize(pToViewDirection + pLightDir);
    float hdotN = dot(halfWay, pNormalVector);
    float specular = pow(max(hdotN, 0.f), pMat.shiness); //Specular 에서 라이트 컬러로 변경
    
    return specular * pLightIntensity;
}


//InRadiance는 포인트라이트 계열에만 동작함. 아닌경우 1,1,1 이다.
float3 PBR(float3 InLightVec, float3 InPixelToViewVector, float3 InNormalWorld, float3 InAlbedo, float InMetalic, float InRoughness, float3 InRadiance)
{
    float3 halfway = normalize(InPixelToViewVector + InLightVec);
        
    float NdotI = max(0.0, dot(InNormalWorld, InLightVec));
    float NdotH = max(0.0, dot(InNormalWorld, halfway));
    float NdotO = max(0.0, dot(InNormalWorld, InPixelToViewVector));
        
    const float3 Fdielectric = 0.04; // 비금속(Dielectric) 재질의 F0
    float3 F0 = lerp(Fdielectric, InAlbedo, InMetalic);
    float3 F = SchlickFresnel(F0, max(0.0, dot(halfway, InPixelToViewVector)));
    float3 kd = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), InMetalic);
    float3 diffuseBRDF = kd * InAlbedo;

    float D = NdfGGX(NdotH, InRoughness);
    float3 G = SchlickGGX(NdotI, NdotO, InRoughness);
        
        // 방정식 (2), 0으로 나누기 방지
    float3 specularBRDF = (F * D * G) / max(1e-5, 4.0 * NdotI * NdotO);

    //float3 radiance = light[i].radiance * saturate((light[i].fallOffEnd - length(lightVec)) / (light[i].fallOffEnd - light[i].fallOffStart));
    
    return (diffuseBRDF + specularBRDF) * InRadiance * NdotI, 1.f;
}

float3 PhongEquation(float ambient, float diffuse, float specular, float3 lightColor)
{
    return (ambient + diffuse + specular) * lightColor;
}

float3 ComputeDirectionalLightPhongModel(Light pLight, float3 pToViewDirection, float3 pNormalVector, Material pMat)
{
    float3 LightVec = -pLight.LightDir;
    float AppliedIntensity = pLight.LightIntensity * max(dot(LightVec, pNormalVector), 0.f);
 
    float Ambient = pMat.ambientStrength * AppliedIntensity;
    float Diffuse = max(dot(pNormalVector, LightVec), 0.0) * AppliedIntensity;
    float Specular = BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
    return PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
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
    float Specular = BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
    
    return PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
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
    float Specular = BRDFSpecularBlinnPhong(LightVec, pToViewDirection, pNormalVector, AppliedIntensity, pMat);
    
    return PhongEquation(Ambient, Diffuse, Specular, pLight.LightColor);
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
