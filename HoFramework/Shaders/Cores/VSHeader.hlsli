
cbuffer TransformConstBuffer : register(b0)
{
    matrix WorldTransform;
    matrix InverseTransform;
    matrix ViewTransform;
    matrix InvViewTransform;
    matrix ProjTransform;
    matrix InvProjTransform;
    matrix ViewProjTransform;
    matrix InvViewProjTransform;
}