#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "Cube1RenderingObject.h"
#include "Cube2RenderingObject.h"
#include "Cube3RenderingObject.h"
#include "Cube4RenderingObject.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//TODO: 에디터처럼 만들려면 나중에 이걸 외부에서 추가 할 수 있도록 변경 필요.
	RenderingObjects.reserve(4);
	RenderingObjects.push_back(std::make_shared<HCube1RenderingObject>(this));
	RenderingObjects.push_back(std::make_shared<HCube2RenderingObject>(this));
	RenderingObjects.push_back(std::make_shared<HCube3RenderingObject>(this));
	RenderingObjects.push_back(std::make_shared<HCube4RenderingObject>(this));


	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Initialize();
	}

	return true;
}


void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Update();
	}
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

	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Render();
	}
	m_swapChain->Present(1, 0);

}
