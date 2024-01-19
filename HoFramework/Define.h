#pragma once
#include <vector>
#include <string>
#include <directxtk/SimpleMath.h>
#include <iostream>


using namespace std;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;


struct Vertex
{
	Vector3 position;
	Vector3 color;
	Vector2 texCoord;
	Vector3 normal;

	Vertex()
	{
		color = Vector3(1.f);
	}
};


struct TransformConstantBuffer
{
	Matrix ModelTransform;
	Matrix InverseTransform;
	Matrix ViewTransform;
	Matrix ProjectionTransform;

	TransformConstantBuffer()
	{
		ModelTransform = Matrix();
		InverseTransform = Matrix();
		ViewTransform = Matrix();
		ProjectionTransform = Matrix();
	}
};


//30bytes
struct Light
{
	Vector3 LightPos; //12
	Vector3 LightDir; //12
	float LightIntensity; //4 
};

struct Material
{
	Vector3 diffuse;
	Vector3 ambient;
	Vector3 specular;
	float roughness;
};

struct Mesh
{
public:
	vector<Vertex> vertices;
	vector<uint16_t> indices;
	string textureSourceName;
};
