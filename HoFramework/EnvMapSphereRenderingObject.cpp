#include "EnvMapSphereRenderingObject.h"
#include "RenderingLibrary.h"
#include "CubeMapRenderingObject.h"

void HEnvMapSphereRenderingObject::Initialize()
{
	Mesh mesh;
	HRenderingLibrary::MakeSphere(&mesh, 1.f, Vector3(0, 0, 0), 64, 24);

	MeshObject obj;
	obj.mesh = mesh;

	m_meshObjects.push_back(obj);


	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/BaseDiffuse/VertexShaderUsingLight.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/EnvMapping/PixelShaderEnvMapping.hlsl");

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	
	HCubeMapRenderingObject* CubeMap = (HCubeMapRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get());
	ID3D11ShaderResourceView* CubeMapDiffuseTexture = CubeMap->GetSkyboxDiffuse().Get();
	ID3D11ShaderResourceView* CubeMapSpecularTexture = CubeMap->GetSkyboxSpecular().Get();

	ID3D11ShaderResourceView* pixelResources[2] = { CubeMapDiffuseTexture,CubeMapSpecularTexture };
	context->PSSetShaderResources(0, 2, pixelResources);
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	

	Scale(Vector3(0.25f, 0.25f, 0.25f));
	Translate(Vector3(0.f, 0.f, 0.f));

	InitializeInternal();
}

void HEnvMapSphereRenderingObject::Update()
{
	m_ParentRenderModule->GetGlobalCameraRotation().y += 0.01f;
	UpdateInternal();
}

void HEnvMapSphereRenderingObject::Render()
{
	RenderInternal();
}
