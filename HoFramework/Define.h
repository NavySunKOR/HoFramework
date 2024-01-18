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
	Vector3 color;
	Vector2 texCoord;

	Vertex()
	{
		color = Vector3(1.f);
	}
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
	vector<uint16_t> indices;
	string textureSourceName;
};