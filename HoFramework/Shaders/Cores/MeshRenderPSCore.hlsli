#include "../Cores/Shading.hlsli"
#include "../Cores/Lighting.hlsli"

//공통으로 사용할 버퍼들은 여기다가 지정한다.
cbuffer MaterialPSBuffer : register(b0)
{
    Material Mat;
};

cbuffer ViewPixelBuffer : register(b1)
{
    float3 ViewPosition;
    float Dummy1;
};

cbuffer LightingPixelBuffer : register(b2)
{
    Light Lights[3];
};




Texture2D g_textureAlbedo : register(t0);
Texture2D g_textureNormal : register(t1);
Texture2D g_textureHeightMap : register(t2);
Texture2D g_textureAmbientOcclusion : register(t3);
Texture2D g_textureMetallic : register(t4);
Texture2D g_textureRoughness : register(t5);
TextureCube SkyboxDiffuse : register(t6);
TextureCube SkyboxSpecular : register(t7);


SamplerState g_sampler : register(s0);


static const int NUM_POINT_LIGHT = 1;
static const int NUM_SPOTLIGHT = 1;