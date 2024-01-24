#include "CubeLightRenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include "Application.h"
#include <algorithm>


void HCubeLightRenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	HRenderingLibrary::MakeBox(&m_drawingMesh);

	HBaseRenderingObject::Initialize();

	//Shaders
	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShaderUsingLight.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShaderUsingLight.hlsl");

	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/wall.jpg"), m_Texture1, m_Texture1ResourceView) == false)
	{
		cout << "wall doesn't exist! " << endl;
	}
	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/crate.png"), m_Texture2, m_Texture2ResourceView) == false)
	{
		cout << "crate doesn't exist! " << endl;
	}


	HRenderingLibrary::CreateConstantBuffer<TransformConstantBuffer>(device, m_transformConstData, m_transformConstBuffer);
	

	//픽셀 콘스턴트 
	m_PSConstBufferData.UsingLight.LightDir = Vector3(0.f, 0.f, 1.f);
	m_PSConstBufferData.UsingLight.LightIntensity = 1.f;
	m_PSConstBufferData.UsingLight.LightPos = Vector3(0.f,0.f,-2.f);

	m_PSConstBufferData.UsingMat.ambient = Vector3(0.3f, 0.3f, 0.3f);
	m_PSConstBufferData.UsingMat.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	m_PSConstBufferData.UsingMat.shiness = 1.f;
	m_PSConstBufferData.UsingMat.specular = Vector3(1.f);

	
	HRenderingLibrary::CreateConstantBuffer<PSConstantBuffer>(device, m_PSConstBufferData, m_PSConstBuffer);
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());
	

	//그 외에 정의
	Scale(Vector3(0.5f, 0.5f, 0.5f));
	Translate(Vector3(0.f, -0.3f, 1.f));

}

void HCubeLightRenderingObject::Update()
{
	//Rotation
	//RotationYValue += 0.01f;
	//RotationXValue += 0.01f;
	Rotate(Vector3(RotationXValue, RotationYValue,0.f));

	HBaseRenderingObject::Update();

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f,0.f,0.f), m_transformConstData.ViewTransform.Transpose().Invert());
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());

}

void HCubeLightRenderingObject::Render()
{
	// 버텍스/인덱스 버퍼 설정

	HBaseRenderingObject::Render();

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	ID3D11ShaderResourceView* pixelResources[2] = { m_Texture1ResourceView.Get(),
											   m_Texture2ResourceView.Get() };
	context->PSSetShaderResources(0, 2, pixelResources);
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	context->PSSetConstantBuffers(0, 1, m_PSConstBuffer.GetAddressOf());
}
