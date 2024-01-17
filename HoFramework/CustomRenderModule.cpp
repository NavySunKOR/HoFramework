#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//여기서부터 작성 


    InitSampler();

	HRenderingLibrary::MakeBox(&m_drawingMesh);

    // Vertex Buffer
	if (!HRenderingLibrary::CreateVertexBuffer(m_device, &m_drawingMesh, m_vertexBuffer))
		return false;

	//Index Buffer
	if(!HRenderingLibrary::CreateIndexBuffer(m_device, &m_drawingMesh, m_indexBuffer))
		return false;

	//Transform Constant Buffer
	if (!HRenderingLibrary::CreateConstantBuffer(m_device, &m_transformConstData, m_transformConstBuffer))
		return false;

	//Shaders
	if (!HRenderingLibrary::CreateVertexShader(m_device, m_vertexShader,m_vertexInputLayout,L"VertexShader.hlsl"))
		return false;

	if (!HRenderingLibrary::CreatePixelShader(m_device, m_pixelShader, L"PixelShader.hlsl"))
		return false;

	return true;
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;

	m_transformConstData.ModelTransform = (Matrix::CreateScale(0.25f) * Matrix::CreateRotationY(RotationYValue) * Matrix::CreateRotationX(RotationXValue)).Transpose();

	using namespace DirectX;
	m_transformConstData.ViewTransform = XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();

	const float fovAngle = ViewAngleInDeg * (XM_PI / 180.f);
	m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(fovAngle,  (float)m_AppContext->GetScreenWidth() / m_AppContext->GetScreenHeight(), 0.01f, 100.0f);
	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();

	D3D11_MAPPED_SUBRESOURCE ms;
	m_context->Map(m_transformConstBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &m_transformConstData, sizeof(m_transformConstData));
	m_context->Unmap(m_transformConstBuffer.Get(), NULL);

}

void HCustomRenderModule::Render()
{
	HBaseRenderModule::Render();
	//여기서부터 작성 
	// IA: Input-Assembler stage
	// VS: Vertex Shader
	// PS: Pixel Shader
	// RS: Rasterizer stage
	// OM: Output-Merger stage.

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_context->IASetInputLayout(m_vertexInputLayout.Get());
	m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 어떤 쉐이더를 사용할지 설정
	m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
	m_context->VSSetConstantBuffers(0, 1, m_transformConstBuffer.GetAddressOf());
	m_context->PSSetShader(m_pixelShader.Get(), 0, 0);

	m_context->RSSetState(m_RasterizerState.Get());
	m_context->RSSetViewports(1, &m_screenViewport);

	// 비교: Depth Buffer를 사용하지 않는 경우
	// m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	m_context->DrawIndexed(m_drawingMesh.indices.size(), 0, 0);


	m_swapChain->Present(1, 0);
}

void HCustomRenderModule::DrawCube1()
{

}

void HCustomRenderModule::DrawCube2()
{

}
