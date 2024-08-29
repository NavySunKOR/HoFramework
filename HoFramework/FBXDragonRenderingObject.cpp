#include "FBXDragonRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HFBXDragonRenderingObject::Initialize()
{
	auto meshes = HRenderingLibrary::LoadMeshFromFile("./Meshes/zeldaPosed001/","zeldaPosed001.fbx");
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ComPtr<ID3D11Device>& Device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(Device, m_vertexShader, m_vertexInputLayout, L"./Shaders/Dragon/VertexShaderDragon.hlsl", HRenderingLibrary::GetVSInputLayout());
	HRenderingLibrary::CreatePixelShader(Device, m_pixelShader, L"./Shaders/Dragon/PixelShaderDragon.hlsl");


	m_PSExtraData.RimIntensity = 5.f;
	m_PSExtraData.RimPow = 5.f;

	HRenderingLibrary::CreateConstantBuffer(Device, m_PSExtraData, m_PSExtraBuffer);

	m_meshObjects.reserve(meshes.size());
	for (Mesh mesh : meshes)
	{
		MeshObject mObj;
		mObj.mesh = mesh;
		m_meshObjects.push_back(mObj);
	}

	InitializeInternal();
}

void HFBXDragonRenderingObject::Update()
{
	UpdateInternal();
}

void HFBXDragonRenderingObject::Render()
{
	//TODO: 콘스탄트 버퍼 설정할 방법 찾아볼것
	/**/
	RenderInternal();
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->PSSetConstantBuffers(1, 1, m_PSExtraBuffer.GetAddressOf()); // 베이스 버퍼 제외시킴
}
