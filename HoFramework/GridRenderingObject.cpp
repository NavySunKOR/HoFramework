#include "GridRenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include <algorithm>
using namespace std;
void HGridRenderingObject::Initialize()
{
	HRenderingLibrary::MakeGrid(&m_drawingMesh, 25, 20, 1.5f);
	HBaseRenderingObject::Initialize();

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/Grid/VertexShaderGridWave.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/Grid/PixelShaderGridDraw.hlsl");

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/map.png"), m_Texture, m_TextureResourceView) == false)
	{
		cout << "wall doesn't exist! " << endl;
	}

	m_PSConstBufferData.UsingLight.LightDir = Vector3(0.f, 0.f, 1.f);
	m_PSConstBufferData.UsingLight.LightIntensity = 1.f;
	m_PSConstBufferData.UsingLight.LightPos = Vector3(0.f, 0.f, -2.f);

	m_PSConstBufferData.UsingMat.ambient = Vector3(0.3f, 0.3f, 0.3f);
	m_PSConstBufferData.UsingMat.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	m_PSConstBufferData.UsingMat.shiness = 1.f;
	m_PSConstBufferData.UsingMat.specular = Vector3(1.f);

	HRenderingLibrary::CreateConstantBuffer<PSConstantBufferGrid>(device, m_PSConstBufferData, m_PSConstBuffer);
	context->PSSetConstantBuffers(0, 1, m_PSConstBuffer.GetAddressOf());

	HRenderingLibrary::CreateConstantBuffer<VSConstantTimeBuffer>(device, m_VSTimeBufferData, m_VSTimeBuffer);
	context->VSSetConstantBuffers(1, 1, m_VSTimeBuffer.GetAddressOf());

	Scale(Vector3(0.1f, 0.1f, 0.1f));
	Translate(Vector3(-1.5f, 0.5f, 1.f));
}

void HGridRenderingObject::Update()
{
	HBaseRenderingObject::Update();

	m_VSTimeBufferData.elapsedTime += 0.1f;
	m_VSTimeBufferData.elapsedTime = (m_VSTimeBufferData.elapsedTime > 6.28f) ? 0 : m_VSTimeBufferData.elapsedTime;
	HRenderingLibrary::UpdateConstantBuffer(m_VSTimeBufferData , m_VSTimeBuffer, m_ParentRenderModule->GetContext());

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_transformConstData.ViewTransform.Transpose().Invert());
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());
}

void HGridRenderingObject::Render()
{
	HBaseRenderingObject::Render();

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	ID3D11ShaderResourceView* pixelResources[1] = { m_TextureResourceView.Get()};
	context->PSSetShaderResources(0, 1, pixelResources);
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	
}
