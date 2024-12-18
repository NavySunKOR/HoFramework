#ifndef CORE
#define CORE

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
    
    float4 customAlbedo;
    
    bool useAlbedoMap;
    bool useNormalMap;
    bool useHeightMap;
    bool useAmbientOcclusionMap;
    
    bool useMetallicMap;
    bool useRoughnessMap;
    bool useIBL;
    bool usePBR;
};

static const float PI = 3.14159265359f;

//Falloff를 0~1사이로 리턴
float GetFallOffAttenutation(float CurrentValue, float InFalloffStart, float InFalloffEnd)
{
    return saturate((InFalloffEnd - CurrentValue) / (InFalloffEnd - InFalloffStart));
}

float3 SchlickFresnel(float3 F0, float NdotH)
{
    float3 ResultFresnel = F0 + (1 - F0) * pow(clamp(1.f - NdotH, 0.F, 1.f),5.f);
    return ResultFresnel;
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

#endif