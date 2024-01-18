
cbuffer TextureCoordMove : register(b0)
{
    float2 TexCoord;
};

SamplerState g_sampler : register(s0);

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 NewColor = input.Color;
    NewColor.xy = input.TexCoord + TexCoord;
    return float4(NewColor, 1.f);
}