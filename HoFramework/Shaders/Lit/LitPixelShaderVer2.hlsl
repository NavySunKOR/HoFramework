#include "../Cores/Core.hlsli"

cbuffer MaterialPSBuffer : register(b0)
{
    Material Mat;
};

cbuffer ViewPixelBuffer : register(b1)
{
    float3 ViewPosition;
    float Dummy;
};

cbuffer LightingPixelBuffer : register(b2)
{
    Light Lights[3];
};


Texture2D g_textureBaseColor : register(t0);
Texture2D g_textureNormal : register(t1);
TextureCube SkyboxDiffuse : register(t2);
TextureCube SkyboxSpecular : register(t3);
SamplerState g_sampler : register(s0);

static const int NUM_POINT_LIGHT = 1;
static const int NUM_SPOTLIGHT = 1;


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    float4 textureColor = g_textureBaseColor.Sample(g_sampler, input.TexCoord);
    float3 textureNormal = g_textureNormal.Sample(g_sampler, input.TexCoord) * 2.f - 1;
    
    float3 N = input.Normal;
    
    //Normal �ʿ� ���������� ���� ź��Ʈ�� ���ش�.
    float3 T = normalize(input.Tangent - dot(input.Tangent, N) * N );
    float3 B = cross(N, T);
        
    
    float3x3 TBN = float3x3(T,B,N);
    float3 newNormal = mul(textureNormal, TBN);
    newNormal = normalize(newNormal);
    input.Normal = newNormal;
    
    
    float4 LightColor = float4(0, 0, 0, 1);
    
    LightColor += float4(ComputeDirectionalLight(Lights[0], toViewDirection, input.Normal, Mat), 1.f);
    
    int i = 1;
    [unroll]
    for (i = 1; i < 1 + NUM_POINT_LIGHT; ++i)
    {
        LightColor += float4(ComputePointLight(Lights[1], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
    }
    
    
    [unroll]
    for (i = 1 + NUM_POINT_LIGHT; i < 1 + NUM_POINT_LIGHT + NUM_SPOTLIGHT; ++i)
    {
        LightColor += float4(ComputeSpotLight(Lights[2], input.WorldPosition, toViewDirection, input.Normal, Mat), 1.f);
    }
    
   
    float3 reflection = reflect(toViewDirection, input.Normal);

    //LightColor += SkyboxDiffuse.Sample(g_sampler, input.Normal) + SkyboxSpecular.Sample(g_sampler, normalize(reflection));
    
    
    return LightColor * textureColor;

    //return textureColor;

}