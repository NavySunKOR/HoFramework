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
	Vector3 tangent; //Tangent Model

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


static_assert((sizeof(BasicVSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

//48bytes
struct Light
{
	Vector3 LightPos = Vector3(0.f, 0.f, 0.f); 
	float LightIntensity = 0.f; 
	Vector3 LightDir = Vector3(0.f, 0.f, 1.f);
	float FalloffStart = 0.f;
	float FalloffEnd = 5.f;
	float SpotFactor = 0.f;

	float Dummy1;
	float Dummy2;
};

//48
struct Material
{
	//16
	//���� - 16byte�� ��� �϶� 
	Vector3 diffuse = Vector3(1.f, 1.f, 1.f);
	float shiness = 1.f;
	
	//�̷��� �� ���� 16����Ʈ�� ���� �־�� �Ѵ� Vector3�� ���޾� ���� ���� Vector3 ������ x ��Ʈ�� ���� ����� �ȴ�.

	//16
	Vector3 ambient = Vector3(0.1f, 0.1f, 0.1f);

	//16
	Vector3 specular = Vector3(1.f);

	float Dummy1;
	float Dummy2;
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
	ComPtr<ID3D11Buffer> materialPSConstantBuffer;
	ComPtr<ID3D11Texture2D> texture;
	ComPtr<ID3D11ShaderResourceView> textureResourceView;

};

struct LightingPSConstantBuffer
{
	Light Lights[3];
};

struct MaterialPSConstantBuffer
{
	Material Mat;
};

struct ViewPSConstantBuffer
{
	Vector3 UsingViewPosition;
	float Dummy;
};

static_assert((sizeof(LightingPSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

static_assert((sizeof(MaterialPSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

static_assert((sizeof(ViewPSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

enum EPrimitiveType
{
	Box = 0,
	Sphere,
	Square,
	Cylinder
};