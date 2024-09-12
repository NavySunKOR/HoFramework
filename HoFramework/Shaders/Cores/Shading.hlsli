#include "../Cores/Core.hlsli"
#ifndef SHADING
#define SHADING
namespace Shading
{
    namespace PBR
    {
        namespace Distribution
        {
            //Normal Distribution
            float DistributionGGX(float NdotH, float roughness)
            {
                float a2 = (roughness * roughness);
                float NdotH2 = NdotH * NdotH;
                float denom = (NdotH2 * (a2 - 1.f) + 1.f);
                float dh = a2 / (PI * denom * denom);
    
                return dh;
            }

            
        }
        
        namespace Geometry
        {  
            float SchlickGGX(float dotResult, float roughness)
            {
    
                float r = (roughness + 1.f);
                float k = (r * r) / 8.f;
    
                return dotResult / (dotResult * (1.f - k) + k);
            }
            
            float GeometrySmith(float NdotL, float NdotO, float Roughness)
            {
                return SchlickGGX(NdotL, Roughness) * SchlickGGX(NdotO, Roughness);

            }
        }
        
        namespace Diffuse
        {
            //Lambert Diffuse = color / pi
            float3 LambertDiffuse(float3 InAlbdeo)
            {
                return InAlbdeo / PI;
            }

        }
        
        namespace Specular
        {
            float3 CookTorranceSpecular(float HdotO, float NdotL, float NdotH, float NdotO, float InRoughness, float3 F)
            {
                float D = Distribution::DistributionGGX(NdotH, InRoughness);
                float3 G = Geometry::GeometrySmith(NdotL, NdotO, InRoughness);
                float3 specularBRDF = (F * D * G) / max(1e-5, 4.0f * NdotL * NdotO);
                return specularBRDF;
            }
        }
        
//InRadiance는 포인트라이트 계열에만 동작함. 아닌경우 1,1,1 이다.
        float3 PBR(float3 InLightVec, float3 InPixelToViewVector, float3 InNormalWorld, float3 InAlbedo, float InMetalic, float InRoughness, float3 InRadiance)
        {
            float3 halfway = normalize(InPixelToViewVector + InLightVec);
        
            float HdotO = max(0.0, dot(halfway, InPixelToViewVector));
            float NdotL = max(0.0, dot(InNormalWorld, InLightVec));
            float NdotH = max(0.0, dot(InNormalWorld, halfway));
            float NdotO = max(0.0, dot(InNormalWorld, InPixelToViewVector));
        
            const float3 Fdielectric = 0.04f; // 비금속(Dielectric) 재질의 F0
            float3 F0 = lerp(Fdielectric, InAlbedo, InMetalic);
            float3 F = SchlickFresnel(F0, HdotO);
                
            float3 kD = (1.f - F) * (1.f - InMetalic);
                
            float3 diffuseBRDF = kD * Diffuse::LambertDiffuse(InAlbedo);
            float3 specularBRDF = Specular::CookTorranceSpecular(HdotO, NdotL, NdotH, NdotO, InRoughness, F);
           
            return (diffuseBRDF + specularBRDF) * InRadiance * NdotL;
        }
    }
    
    namespace Phong
    {
        float BRDFSpecularBlinnPhong(float3 pLightDir, float3 pToViewDirection, float3 pNormalVector, float pLightIntensity, Material pMat)
        {
            float3 halfWay = normalize(pToViewDirection + pLightDir);
            float hdotN = dot(halfWay, pNormalVector);
            float specular = pow(max(hdotN, 0.f), pMat.shiness); //Specular 에서 라이트 컬러로 변경
    
            return specular * pLightIntensity;
        }
        
        float3 PhongEquation(float ambient, float diffuse, float specular, float3 lightColor)
        {
            return (ambient + diffuse + specular) * lightColor;
        }
    }
    

}
#endif