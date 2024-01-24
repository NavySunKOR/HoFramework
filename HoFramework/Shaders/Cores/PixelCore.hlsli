#include "Lights.hlsli"

cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float Dummy; // 4byte
    Material UsingMat; //48Byte
};