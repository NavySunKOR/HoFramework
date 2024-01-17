#pragma once
#include <vector>
#include <string>
#include <directxtk/SimpleMath.h>


using namespace std;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector3 tangent;
	Vector3 color;
	Vector2 uv;
};


struct TransformConstantBuffer
{
	Matrix ModelTransform;
	Matrix ViewTransform;
	Matrix ProjectionTransform;

	TransformConstantBuffer()
	{
		ModelTransform = Matrix();
		ViewTransform = Matrix();
		ProjectionTransform = Matrix();
	}
};

struct Mesh
{
public:
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	string textureSourceName;
};