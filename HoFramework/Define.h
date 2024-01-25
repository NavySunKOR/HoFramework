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
	//���� - 16byte�� ��� �϶� 
	Vector3 diffuse;
	float shiness;
	
	//�̷��� �� ���� 16����Ʈ�� ���� �־�� �Ѵ� Vector3�� ���޾� ���� ���� Vector3 ������ x ��Ʈ�� ���� ����� �ȴ�.

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