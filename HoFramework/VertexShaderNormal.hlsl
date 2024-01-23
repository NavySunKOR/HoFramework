#include "VertexCore.hlsli"

PSInput main(VSInput Input)
{
    PSInput Output;
    float4 Position = float4(Input.Position, 1.f);
    
    Output.Normal = mul(float4(Input.Normal, 0), InverseTransform).xyz;
    Output.Normal = normalize(Output.Normal);
    
    float t = Input.TexCoord.x;
    
    Position = mul(Position, ModelTransform);
    Position.xyz += Output.Normal * t * 1.f;
    Output.WorldPosition = Position.xyz;
    
    Position = mul(Position, ViewTransform);
    Position = mul(Position, ProjectionTransform);
    Output.ScreenPosition = Position;
    
    Output.TexCoord = Input.TexCoord;
    Output.Color = float3(1, 0, 0) * t + float3(1, 0, 1) * (1 - t);
    
    return Output;
}