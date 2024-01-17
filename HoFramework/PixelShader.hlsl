struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.Color, 1.f);
}