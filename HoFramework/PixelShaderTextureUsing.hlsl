Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
SamplerState g_sampler : register(s0);

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    return (input.TexCoord.x < 0.5f)? g_texture0.Sample(g_sampler, input.TexCoord) : g_texture1.Sample(g_sampler, input.TexCoord + float2(0.5, 0));
    //return g_texture0.Sample(g_sampler, input.TexCoord);

}