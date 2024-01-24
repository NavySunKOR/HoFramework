#include "../Cores/Core.hlsli"


cbuffer VSTimeBuffer : register(b1)
{
    float elapsedTime;
    float3 DummyVSTimeBuffer1;
}

PSInput main(VSInput Input)
{
    PSInput Output;
    Output.Normal = mul(float4(Input.Normal, 0), InverseTransform).xyz;
    Output.Normal = normalize(Output.Normal);
    
    float4 Position = float4(Input.Position, 1.f);
    
    float Amplitue = 2.f;
    float Frequency = 20.f;
    
    Position += float4(Input.Normal * sin(Input.TexCoord.x * Frequency + elapsedTime) * Amplitue, 1);
    
    Position = mul(Position, ModelTransform);
    Output.WorldPosition = Position.xyz;
    
    Position = mul(Position, ViewTransform);
    Position = mul(Position, ProjectionTransform);
    Output.ScreenPosition = Position;
    
    
    Output.Color = Input.Color;
    Output.TexCoord = Input.TexCoord;
    
    
    
    
    return Output;
}