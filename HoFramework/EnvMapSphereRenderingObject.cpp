#include "EnvMapSphereRenderingObject.h"
#include "RenderingLibrary.h"
#include "CubeMapRenderingObject.h"

void HEnvMapSphereRenderingObject::Initialize()
{
	HRenderingLibrary::MakeSphere(&m_drawingMesh, 1.f, Vector3(0, 0, 0), 12, 24);
	HBaseRenderingObject::Initialize();



	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/BaseDiffuse/VertexShaderUsingLight.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/EnvMapping/PixelShaderEnvMapping.hlsl");

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	m_PSConstBufferData.UsingLight.LightDir = Vector3(0.f, 0.f, 1.f);
	m_PSConstBufferData.UsingLight.LightIntensity = 1.f;
	m_PSConstBufferData.UsingLight.LightPos = Vector3(0.f, 0.f, -2.f);

	m_PSConstBufferData.UsingMat.ambient = Vector3(0.3f, 0.3f, 0.3f);
	m_PSConstBufferData.UsingMat.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	m_PSConstBufferData.UsingMat.shiness = 1.f;
	m_PSConstBufferData.UsingMat.specular = Vector3(1.f);

	HRenderingLibrary::CreateConstantBuffer<PSConstantBuffer>(device, m_PSConstBufferData, m_PSConstBuffer);
	context->PSSetConstantBuffers(0, 1, m_PSConstBuffer.GetAddressOf());
	
	HCubeMapRenderingObject* CubeMap = (HCubeMapRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get());
	ID3D11ShaderResourceView* CubeMapTexture = CubeMap->GetSkyboxTexture().Get();

	ID3D11ShaderResourceView* pixelResources[1] = { CubeMapTexture };
	context->PSSetShaderResources(0, 1, pixelResources);
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	

	Scale(Vector3(0.25f, 0.25f, 0.25f));
	Translate(Vector3(0.f, 0.f, 0.f));
}

void HEnvMapSphereRenderingObject::Update()
{
	m_ParentRenderModule->GetGlobalCameraRotation().y += 0.01f;
	HBaseRenderingObject::Update();

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_transformConstData.ViewTransform.Transpose().Invert());
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());
}

void HEnvMapSphereRenderingObject::Render()
{
	HBaseRenderingObject::Render();
}
