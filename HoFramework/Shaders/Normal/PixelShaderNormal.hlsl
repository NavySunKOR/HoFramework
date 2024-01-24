#include "../Cores/PixelCore.hlsli"

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.Color,1);
}