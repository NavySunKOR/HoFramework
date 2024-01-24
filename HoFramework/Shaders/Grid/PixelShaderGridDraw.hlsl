#include "../Cores/Core.hlsli"


float4 main(PSInput input) : SV_TARGET
{
    float3 toViewDirection = normalize(ViewPosition - input.WorldPosition);
    
    float4 LightColor = float4(ComputeDirectionalLight(UsingLight, toViewDirection, input.Normal, UsingMat), 1.f);
    return float4(1.f, 1.f, 1.f,1.f);

}