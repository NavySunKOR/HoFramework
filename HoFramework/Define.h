#pragma once
#include <vector>
#include <string>
#include <directxtk/SimpleMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <wrl.h> // ComPtr


using namespace std;
using Microsoft::WRL::ComPtr;
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


struct BasicVSConstantBuffer
{
	Matrix ModelTransform;
	Matrix InverseTransform;
	Matrix ViewTransform;
	Matrix ProjectionTransform;

	BasicVSConstantBuffer()
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
	Vector3 LightPos = Vector3(0.f, 0.f, -2.f); //12
	float LightIntensity = 1.f; //4 

	//16
	Vector3 LightDir = Vector3(0.f, 0.f, 1.f); //12
	float Dummy; //4 
};

//48
struct Material
{
	//16
	//주의 - 16byte씩 페어 일때 
	Vector3 diffuse = Vector3(0.5f, 0.5f, 0.5f);
	float shiness = 1.f;
	
	//이렇게 한 쌍이 16바이트로 묶여 있어야 한다 Vector3를 연달아 쓰면 다음 Vector3 변수의 x 파트에 덮어 씌우게 된다.

	//16
	Vector3 ambient = Vector3(0.3f, 0.3f, 0.3f);
	float dummy1;

	//16
	Vector3 specular = Vector3(1.f);
	float dummy2;

};


struct Mesh
{
public:
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	string textureSourceName;
};

struct MeshObject
{
public:
	Mesh mesh;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> vertexConstantBuffer;
	ComPtr<ID3D11Buffer> pixelConstantBuffer;
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> textureResourceView;

};

struct BasicPSConstantBuffer
{

	//32
	Light UsingLight;

	//12
	Vector3 UsingViewPosition;

	float Dummy;

	//
	Material UsingMat;
};

enum EPrimitiveType
{
	Box = 0,
	Sphere,
	Square,
	Cylinder
};