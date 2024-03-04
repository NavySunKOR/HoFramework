#include "LoadFBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HLoadFBXRenderingObject::Initialize()
{
	auto loaded = HRenderingLibrary::LoadMeshFromFile("./Meshes/zeldaPosed001/", "zeldaPosed001.fbx");
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ComPtr<ID3D11Device>& Device = m_ParentRenderModule->GetDevice();

	HRenderingLibrary::CreateVertexShader(Device, m_vertexShader, m_vertexInputLayout, L"./Shaders/FBX/VertexShaderUsingLight.hlsl", HRenderingLibrary::GetVSInputLayout());
	HRenderingLibrary::CreatePixelShader(Device, m_pixelShader, L"./Shaders/FBX/PixelShaderUsingLight.hlsl");

	Scale(Vector3(0.01f, 0.01f, 0.01f));
	Translate(Vector3(0.f, -1.f, 0.f));

	m_meshObjects.reserve(loaded.size());

	for (Mesh mesh : loaded)
	{
		MeshObject newObj;
		newObj.mesh = mesh;
		m_meshObjects.push_back(newObj);
	}

	InitializeInternal();
}

void HLoadFBXRenderingObject::Update()
{
	UpdateInternal();
}

void HLoadFBXRenderingObject::Render()
{
	RenderInternal();
}
