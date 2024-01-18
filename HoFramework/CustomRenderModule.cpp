#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "Cube1RenderingObject.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//���⼭���� �ۼ� 
    InitSampler();

	Cube1 = new HCube1RenderingObject(this);
	Cube1->Initialize();

	return true;
}

HCustomRenderModule::~HCustomRenderModule()
{
	delete Cube1;
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//���⼭���� �ۼ� 
	Cube1->Update();

}

void HCustomRenderModule::Render()
{
	HBaseRenderModule::Render();
	//���⼭���� �ۼ� 
	// IA: Input-Assembler stage
	// VS: Vertex Shader
	// PS: Pixel Shader
	// RS: Rasterizer stage
	// OM: Output-Merger stage.

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_context->RSSetState(m_RasterizerState.Get());
	m_context->RSSetViewports(1, &m_screenViewport);

	// ��: Depth Buffer�� ������� �ʴ� ���
	// m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	Cube1->Render();

	m_swapChain->Present(1, 0);
}
