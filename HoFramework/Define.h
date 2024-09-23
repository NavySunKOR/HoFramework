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
using DirectX::SimpleMath::Vector4;
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
	Matrix WorldTransform;
	Matrix InverseTransform;
	Matrix ViewTransform;
	Matrix InvViewTransform;
	Matrix ProjTransform;
	Matrix InvProjTransform;
	Matrix ViewProjTransform;
	Matrix InvViewProjTransform;

	BasicVSConstantBuffer()
	{
		WorldTransform = Matrix();
		InverseTransform = Matrix();
		ViewTransform = Matrix();
		InvViewTransform = Matrix();
		ProjTransform = Matrix();
		InvProjTransform = Matrix();
		ViewProjTransform = Matrix();
		InvViewProjTransform = Matrix();
	}
};


static_assert((sizeof(BasicVSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

//48
struct Material
{
	float AmbientStrength = 0.f;
	float Roughness = 0.f;
	float Metalic = 0.f;
	float Shiness = 3.f;

	Vector4 FresnelR0 = Vector4(0.4f,0.4f,0.4f,1.f);

	Vector4 CustomAlbedo = Vector4(0.5f, 0.5f, 0.5f,1.f);

	int UseAlbedoMap = 0;
	int UseNormalMap = 0;
	int UseHeightMap = 0;
	int UseAmbientOcclusionMap = 0;

	int UseMetallicMap = 0;
	int UseRoughnessMap = 0;
	int UseIBL = 0;
	int UsePBR = 1;
};


struct Mesh
{
public:
	vector<Vertex> vertices;
	vector<uint32_t> indices;
	string textureNames[6];
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


static_assert((sizeof(MaterialPSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

static_assert((sizeof(ViewPSConstantBuffer) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");


struct MeshObject
{
public:
	Mesh mesh;
	MaterialPSConstantBuffer materialPSConstantData;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Buffer> vertexConstantBuffer;
	ComPtr<ID3D11Buffer> materialPSConstantBuffer;
	ComPtr<ID3D11Texture2D> textures[6];
	ComPtr<ID3D11ShaderResourceView> textureResourceViews[6];
};


enum EPrimitiveType
{
	Box = 0,
	Sphere,
	Square,
	Cylinder
};


enum EModelTexture2DType
{
	Albedo = 0,
	Normal,
	Height,
	AmbientOcclusion,
	Metallic,
	Roughness
};

enum EModelTextureCubType
{
	IBLRadiance,
	IBLIrradiance
};
