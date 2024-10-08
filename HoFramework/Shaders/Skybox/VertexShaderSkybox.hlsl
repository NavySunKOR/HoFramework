#include "../Cores/Core.hlsli"
#include "../Cores/VSHeader.hlsli"

PSInput main(VSInput Input)
{
    PSInput Output;
    float4 Position = float4(Input.Position, 1.f);
    Output.ModelPosition = Position;

    Position = mul(Position, WorldTransform);

    Output.WorldPosition = Position.xyz;

    Position = mul(Position, ViewTransform);
    Position = mul(Position, ProjTransform);
    Output.ScreenPosition = Position;

    Output.Color = Input.Color;
    Output.TexCoord = Input.TexCoord;
    Output.Normal = mul(float4(Input.Normal, 0), InverseTransform).xyz;
    Output.Normal = normalize(Output.Normal);

    return Output;
}