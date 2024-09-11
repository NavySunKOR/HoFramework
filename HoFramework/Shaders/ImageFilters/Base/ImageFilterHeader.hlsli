


Texture2D<float4> g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

static const float gaussianBlurWeights[5] = { 0.0545, 0.2442, 0.4026, 0.2442, 0.0545 };

struct SamplingVertexShaderInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct SamplingPixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};


float4 LinearToneMapping(float4 InColor, float InExposure, float InGamma)
{
    float4 invGamma = float4(1.f, 1.f, 1.f, 1.f) / InGamma;
    InColor = clamp(InColor * InExposure, 0, 1);
    InColor = pow(InColor, invGamma);

    InColor.a = 1.f;
    return InColor;
}