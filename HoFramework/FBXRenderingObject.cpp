#include "FBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HFBXRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	ComPtr<ID3D11Device>& Device = m_ParentRenderModule->GetDevice();
	m_PSExtraData.RimIntensity = 5.f;
	m_PSExtraData.RimPow = 5.f;
	HRenderingLibrary::CreateConstantBuffer(Device, m_PSExtraData, m_PSExtraBuffer);
	InitializeInternal();
}

void HFBXRenderingObject::Update()
{
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
