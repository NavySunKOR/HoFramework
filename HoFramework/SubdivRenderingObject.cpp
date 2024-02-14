#include "SubdivRenderingObject.h"

void HSubdivRenderingObject::Initialize()
{
	HRenderingLibrary::MakeSphere(&m_drawingMesh,3.f,Vector3(0,0,0),3,3);
	HRenderingLibrary::MakeSphereSubdivision(&m_drawingMesh, 3.f);
	HRenderingLibrary::MakeSphereSubdivision(&m_drawingMesh, 3.f);
	HRenderingLibrary::MakeSphereSubdivision(&m_drawingMesh, 3.f);

	HBaseRenderingObject::Initialize();
	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/BaseDiffuse/VertexShaderUsingLight.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/Subdiv/PixelShaderSubdivision.hlsl");

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


	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/map.png"), m_Texture, m_TextureResourceView) == false)
	{
		cout << "wall doesn't exist! " << endl;
	}

	Scale(Vector3(0.25f, 0.25f, 0.25f));
	Translate(Vector3(0.f, 0.f, 0.f));
}

void HSubdivRenderingObject::Update()
{
	RotX += 0.01f;
	Rotate(Vector3(0, RotX, 0));
	HBaseRenderingObject::Update();

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_transformConstData.ViewTransform.Transpose().Invert());
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());
}

void HSubdivRenderingObject::Render()
{
	HBaseRenderingObject::Render();
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	context->PSSetShaderResources(0, 1, { m_TextureResourceView.GetAddressOf() });
	context->PSSetConstantBuffers(0, 1, m_PSConstBuffer.GetAddressOf());
}
