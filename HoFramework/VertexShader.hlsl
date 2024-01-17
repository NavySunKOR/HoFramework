
cbuffer TransformConstBuffer : register(b0)
{
    matrix ModelTransform;
    matrix ViewTransform;
    matrix ProjectionTransform;
}


struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
};

PSInput main(VSInput Input)
{
    PSInput Output;
    float4 Position = float4(Input.Position, 1.f);
    Position = mul(Position, ModelTransform);
    Position = mul(Position, ViewTransform);
    Position = mul(Position, ProjectionTransform);
    
    Output.Position = Position;
    Output.Color = Input.Color;
    
    return Output;
}