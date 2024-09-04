#include "../Cores/Core.hlsli"

//�������� ����� ���۵��� ����ٰ� �����Ѵ�.
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