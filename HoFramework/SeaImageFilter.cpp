#include "SeaImageFilter.h"
#include "RenderingLibrary.h"

void HSeaImageFilter::Initialize(HBaseRenderModule* ParentModule, const wstring vertexShaderName, const wstring pixelShaderName, int width, int height)
{
	HImageFilter::Initialize(ParentModule, vertexShaderName, pixelShaderName, width, height);

	HRenderingLibrary::CreateConstantBuffer<SeaPixelConstantBuffer>(m_parentRenderModule->GetDevice(), Sea, m_SeaPixelConstantBuffer);
}

void HSeaImageFilter::Update()
{
	Sea.iTime += 0.016f;
	HRenderingLibrary::UpdateConstantBuffer<SeaPixelConstantBuffer>(Sea, m_SeaPixelConstantBuffer, m_parentRenderModule->GetContext());
	HImageFilter::Update();
}

void HSeaImageFilter::Render()
{
	HImageFilter::Render();
	
	m_parentRenderModule->GetContext()->PSSetConstantBuffers(1,1, m_SeaPixelConstantBuffer.GetAddressOf());

}
