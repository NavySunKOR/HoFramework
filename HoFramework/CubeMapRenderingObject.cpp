#include "CubeMapRenderingObject.h"
#include "RenderingLibrary.h"
#include <directxtk/DDSTextureLoader.h> 
#include <directxtk/WICTextureLoader.h>


using namespace DirectX;

void HCubeMapRenderingObject::Initialize()
{
	Mesh mesh;
	HRenderingLibrary::MakeBox(&mesh,20);
	
	MeshObject obj;
	obj.mesh = mesh;

	m_meshObjects.push_back(obj);

	
	auto hr = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), L"./skybox/clearSky/CloudCommon_diffuseIBL.dds", 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxDiffuseResource.GetAddressOf(),
		m_SkyboxDiffuseResourceView.GetAddressOf(), nullptr);

	auto hr2 = CreateDDSTextureFromFileEx(
		m_ParentRenderModule->GetDevice().Get(), L"./skybox/clearSky/CloudCommons_specularIBL.dds", 0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
		DDS_LOADER_FLAGS(false), (ID3D11Resource**)m_SkyboxSpecularResource.GetAddressOf(),
		m_SkyboxSpecularResourceView.GetAddressOf(), nullptr);

	HRenderingLibrary::CreateVertexShader(m_ParentRenderModule->GetDevice(), m_vertexShader, m_vertexInputLayout, L"./Shaders/Skybox/VertexShaderSkybox.hlsl", HRenderingLibrary::GetVSInputLayout());
	HRenderingLibrary::CreatePixelShader(m_ParentRenderModule->GetDevice(), m_pixelShader, L"./Shaders/Skybox/PixelShaderSkybox.hlsl");


	InitializeInternal();

}

void HCubeMapRenderingObject::Update()
{
	UpdateInternal();
}

void HCubeMapRenderingObject::Render()
{
	ComPtr<ID3D11DeviceContext> Context = m_ParentRenderModule->GetContext();
	ID3D11ShaderResourceView* resources[1] = { m_SkyboxSpecularResourceView.Get()};
	Context->PSSetShaderResources(0, 1, resources);
	Context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	RenderInternal();
}
