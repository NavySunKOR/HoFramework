#include "CubeMapRenderingObject.h"
#include "RenderingLibrary.h"
#include <directxtk/DDSTextureLoader.h> 
#include <directxtk/WICTextureLoader.h>


using namespace DirectX;

void HCubeMapRenderingObject::Initialize()
{
	HRenderingLibrary::MakeBox(&m_drawingMesh,20);

	//std::reverse(m_drawingMesh.indices.begin(), m_drawingMesh.indices.end());
	HBaseRenderingObject::Initialize();

	
	auto hr = CreateDDSTextureFromFileEx(
		// this->m_device.Get(), L"./SaintPetersBasilica/saintpeters.dds", 0,
		m_ParentRenderModule->GetDevice().Get(), L"./skybox/clearSky/skybox.dds", 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxResource.GetAddressOf(),
		m_SkyboxResourceView.GetAddressOf(), nullptr);

	HRenderingLibrary::CreateVertexShader(m_ParentRenderModule->GetDevice(), m_vertexShader, m_vertexInputLayout, L"./Shaders/Skybox/VertexShaderSkybox.hlsl", HRenderingLibrary::GetVSInputLayout());
	HRenderingLibrary::CreatePixelShader(m_ParentRenderModule->GetDevice(), m_pixelShader, L"./Shaders/Skybox/PixelShaderSkybox.hlsl");


	//Scale(Vector3(20, 20, 20));

}

void HCubeMapRenderingObject::Update()
{
	HBaseRenderingObject::Update();
}

void HCubeMapRenderingObject::Render()
{
	ComPtr<ID3D11DeviceContext> Context = m_ParentRenderModule->GetContext();
	Context->PSSetShaderResources(0, 1, { m_SkyboxResourceView.GetAddressOf() });
	Context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	HBaseRenderingObject::Render();
}
