Texture2D g_texture0 : register(t0);
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