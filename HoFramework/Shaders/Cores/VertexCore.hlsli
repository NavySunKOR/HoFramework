#include "Core.hlsli"

cbuffer TransformConstBuffer : register(b0)
{
    matrix ModelTransform;
    matrix InverseTransform;
    matrix ViewTransform;
    matrix ProjectionTransform;
}