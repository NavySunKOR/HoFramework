
#include"./ImageFilterHeader.hlsli"


SamplingPixelShaderInput main(SamplingVertexShaderInput input)
{
    SamplingPixelShaderInput output;

    output.position = float4(input.Position, 1.0);
    output.texcoord = input.TexCoord;

    return output;
}
