cbuffer TransformConstBuffer : register(b0)
{
    matrix ModelTransform;
    matrix InverseTransform;
    matrix ViewTransform;
    matrix ProjectionTransform;
}

struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
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
    Output.TexCoord = Input.TexCoord;
    Output.Normal = normalize(mul(float4(Input.Normal, 0), InverseTransform));
   
    return Output;
}