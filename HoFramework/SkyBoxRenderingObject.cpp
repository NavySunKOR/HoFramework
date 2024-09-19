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

void HSkyBoxRenderingObject::Render(HCamera InCamera)
{
	UpdateBuffer(InCamera);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ComPtr<ID3D11DeviceContext> Context = m_ParentRenderModule->GetContext();
	ID3D11ShaderResourceView* resources[1] = { m_SkyboxOriginalResourceView.Get()};
	Context->PSSetShaderResources(0, 1, resources);

	Context->IASetVertexBuffers(0, 1, m_meshObjects[0].vertexBuffer.GetAddressOf(), &stride, &offset);
	Context->IASetIndexBuffer(m_meshObjects[0].indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ¾î¶² ½¦ÀÌ´õ¸¦ »ç¿ëÇÒÁö ¼³Á¤
	Context->VSSetConstantBuffers(0, 1, m_meshObjects[0].vertexConstantBuffer.GetAddressOf());


	Context->PSSetConstantBuffers(0, 1, m_meshObjects[0].materialPSConstantBuffer.GetAddressOf());
	Context->PSSetConstantBuffers(1, 1, m_viewConstBuffer.GetAddressOf());
	Context->PSSetConstantBuffers(2, 1, m_ParentRenderModule->m_LightPSConstantBuffer.GetAddressOf());
	Context->DrawIndexed((UINT)m_meshObjects[0].mesh.indices.size(), 0, 0);
}

void HSkyBoxRenderingObject::SetSkyboxResources(LPCWSTR InOriginalDDSLoc, LPCWSTR InDiffuseDDSLoc, LPCWSTR InSpecularDDSLoc, LPCWSTR InBRDFDDSLoc)
{
	auto hr = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InOriginalDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxOriginalResource.GetAddressOf(),
		m_SkyboxOriginalResourceView.GetAddressOf(), nullptr);

	auto hr2 = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InDiffuseDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxDiffuseResource.GetAddressOf(),
		m_SkyboxDiffuseResourceView.GetAddressOf(), nullptr);

	auto hr3 = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InSpecularDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxSpecularResource.GetAddressOf(),
		m_SkyboxSpecularResourceView.GetAddressOf(), nullptr);

	auto hr4 = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), InBRDFDDSLoc, 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		0, // Texture2D
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxBRDFResource.GetAddressOf(),
		m_SkyboxBRDFResourceView.GetAddressOf(), nullptr);
}
