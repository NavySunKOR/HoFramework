#include "../Cores/Core.hlsli"


cbuffer PixelCalculateBuffer : register(b0)
{
    Light UsingLight; // 30byte
    float3 ViewPosition; //12byte
    float PixelCalculateBufferDummy; // 4byte
    Material UsingMat; //48Byte
};


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    
    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal, UsingMat), 1.f);
    return float4(1.f, 1.f, 1.f,1.f);

}