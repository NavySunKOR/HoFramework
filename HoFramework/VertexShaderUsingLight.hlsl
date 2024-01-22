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
    float4 ScreenPosition : SV_POSITION;
    float3 WorldPosition : POSITION;
    float3 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};


PSInput main(VSInput Input)
{
    PSInput Output;
    float4 Position = float4(Input.Position, 1.f);
    
    Position = mul(Position, ModelTransform);
    Output.WorldPosition = Position.xyz;
    
    Position = mul(Position, ViewTransform);
    Position = mul(Position, ProjectionTransform);
    Output.ScreenPosition = Position;
    
    Output.Color = Input.Color;
    Output.TexCoord = Input.TexCoord;
    Output.Normal = mul(float4(Input.Normal, 0), InverseTransform).xyz;
    Output.Normal = normalize(Output.Normal);
    
    return Output;
}