#include "../Cores/Core.hlsli"

//공통으로 사용할 버퍼들은 여기다가 지정한다.
cbuffer MaterialPSBuffer : register(b0)
{
    Material Mat;
};

cbuffer ViewPixelBuffer : register(b1)
{
    float3 ViewPosition;
    float exposure;

    float3 Dummy1;
    float gamma;
    
};

cbuffer LightingPixelBuffer : register(b2)
{
    Light Lights[3];
};