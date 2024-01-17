#include "CustomRenderModule.h"
#include "RenderingLibrary.h"

void HCustomRenderModule::InitSampler()
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the Sample State
    m_device->CreateSamplerState(&sampDesc, m_SamplerState.GetAddressOf());

    
}

bool HCustomRenderModule::CreateCube1()
{
	HRenderingLibrary::MakeBox(&m_drawingMesh);

	// Vertex Buffer
	if (!HRenderingLibrary::CreateVertexBuffer(m_device, &m_drawingMesh, m_vertexBuffer))
		return false;

	//Index Buffer
	if (!HRenderingLibrary::CreateIndexBuffer(m_device, &m_drawingMesh, m_indexBuffer))
		return false;

	//Transform Constant Buffer
	if (!HRenderingLibrary::CreateConstantBuffer(m_device, &m_transformConstData, m_transformConstBuffer))
		return false;

	//Shaders
	if (!HRenderingLibrary::CreateVertexShader(m_device, m_vertexShader, m_vertexInputLayout, L"VertexShader.hlsl"))
		return false;

	if (!HRenderingLibrary::CreatePixelShader(m_device, m_pixelShader, L"PixelShader.hlsl"))
		return false;

	return true;
}