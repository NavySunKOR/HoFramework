#include "SkyBoxRenderingObject.h"
#include "RenderingLibrary.h"
#include <directxtk/DDSTextureLoader.h> 
#include <directxtk/WICTextureLoader.h>


using namespace DirectX;

void HSkyBoxRenderingObject::Initialize()
{
	InitializeInternal();
}

void HSkyBoxRenderingObject::Update()
{
	UpdateInternal();
}

void HSkyBoxRenderingObject::Render()
{
	ComPtr<ID3D11DeviceContext> Context = m_ParentRenderModule->GetContext();
	ID3D11ShaderResourceView* resources[1] = { m_SkyboxSpecularResourceView.Get()};
	Context->PSSetShaderResources(0, 1, resources);
	Context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	RenderInternal();
}

void HSkyBoxRenderingObject::SetSkyboxResources(LPCWSTR InDiffuseDDSLoc, LPCWSTR InSpecularDDSLoc)
{
	auto hr = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InDiffuseDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxDiffuseResource.GetAddressOf(),
		m_SkyboxDiffuseResourceView.GetAddressOf(), nullptr);

	auto hr2 = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InSpecularDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxSpecularResource.GetAddressOf(),
		m_SkyboxSpecularResourceView.GetAddressOf(), nullptr);

}
