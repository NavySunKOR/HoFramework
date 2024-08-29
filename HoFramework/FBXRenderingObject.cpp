#include "FBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HFBXRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_PSConstBufferData.UsingLight[0].LightIntensity = 1.f;
	m_PSConstBufferData.UsingLight[0].LightDir = Vector3(0.5f,0.25f,0.1f);
	m_PSConstBufferData.UsingLight[0].LightPos = Vector3(0, 0, 0);


	//PointLight
	m_PSConstBufferData.UsingLight[1].LightIntensity = 0.f;
	m_PSConstBufferData.UsingLight[1].LightDir = Vector3(0.5f, 0.25f, 0.1f);
	m_PSConstBufferData.UsingLight[1].LightPos = Vector3(1, 0, 0);
	m_PSConstBufferData.UsingLight[1].FalloffStart = 1.f;
	m_PSConstBufferData.UsingLight[1].FalloffEnd = 3.f;
	m_PSConstBufferData.UsingLight[1].SpotFactor = 2.5f;

	m_PSConstBufferData.UsingLight[2].LightIntensity = 0.f;
	m_PSConstBufferData.UsingLight[2].LightDir = Vector3(0.f, -1.f, 0.f);
	m_PSConstBufferData.UsingLight[2].LightPos = Vector3(0, 1.5f, 0);
	m_PSConstBufferData.UsingLight[2].FalloffStart = 1.f;
	m_PSConstBufferData.UsingLight[2].FalloffEnd = 3.f;
	m_PSConstBufferData.UsingLight[2].SpotFactor = 2.5f;

	ComPtr<ID3D11Device>& Device = m_ParentRenderModule->GetDevice();
	m_PSExtraData.RimIntensity = 5.f;
	m_PSExtraData.RimPow = 5.f;
	HRenderingLibrary::CreateConstantBuffer(Device, m_PSExtraData, m_PSExtraBuffer);
	InitializeInternal();
}

void HFBXRenderingObject::Update()
{
	m_basicPixelConstBufferData = m_PSConstBufferData;
	UpdateInternal();
}

void HFBXRenderingObject::Render()
{
	//TODO: 콘스탄트 버퍼 설정할 방법 찾아볼것
	/**/
	RenderInternal();
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->PSSetConstantBuffers(1, 1, m_PSExtraBuffer.GetAddressOf()); // 베이스 버퍼 제외시킴
}
