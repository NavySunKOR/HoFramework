#include "../Cores/Core.hlsli"
#include "../Cores/VSHeader.hlsli"

float4 main(VSInput input) : SV_POSITION
{
    float ScreenPos = float4(input.Position, 1.f);
    ScreenPos = mul(ScreenPos, WorldTransform);
    ScreenPos = mul(ScreenPos, ViewProjTransform);
	
    return ScreenPos;
}