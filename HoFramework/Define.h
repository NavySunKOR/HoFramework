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


//32bytes
struct Light
{
	//16
	Vector3 LightPos; //12
	float LightIntensity; //4 

	//16
	Vector3 LightDir; //12
	float Dummy; //4 
};

//48
struct Material
{
	//16
	//주의 - 16byte씩 페어 일때 
	Vector3 diffuse;
	float shiness;
	
	//이렇게 한 쌍이 16바이트로 묶여 있어야 한다 Vector3를 연달아 쓰면 다음 Vector3 변수의 x 파트에 덮어 씌우게 된다.

	//16
	Vector3 ambient;
	float dummy1;

	//16
	Vector3 specular;
	float dummy2;

};

struct Mesh
{
public:
	vector<Vertex> vertices;
	vector<uint16_t> indices;
	string textureSourceName;
};


struct PSConstantBuffer
{

	//32
	Light UsingLight;

	//12
	Vector3 UsingViewPosition;

	float Dummy;

	//
	Material UsingMat;
};