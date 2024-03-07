#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "LoadFBXRenderingObject.h"
#include "FBXDragonRenderingObject.h"
#include "CubeMapRenderingObject.h"
#include "EnvMapSphereRenderingObject.h"
#include "ImageFilter.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//TODO: 에디터처럼 만들려면 나중에 이걸 외부에서 추가 할 수 있도록 변경 필요.
	RenderingObjects.reserve(2);
	//RenderingObjects.push_back(std::make_shared<HCubeLightRenderingObject>(this));
	//RenderingObjects.push_back(std::make_shared<HCubeNormalRenderingObject>(this));
	//RenderingObjects.push_back(std::make_shared<HGridRenderingObject>(this));
	//RenderingObjects.push_back(std::make_shared<HCylinderRenderingObject>(this));
	RenderingObjects.push_back(std::make_shared<HCubeMapRenderingObject>(this));
	//RenderingObjects.push_back(std::make_shared<HEnvMapSphereRenderingObject>(this));
	RenderingObjects.push_back(std::make_shared<HFBXDragonRenderingObject>(this));


	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Initialize();
	}

	InitImageFilters();

	if (!GUIRenderSubModule.Initialize(this))
		return true;

	return true;
}


void HCustomRenderModule::InitImageFilters()
{
	ImageFilters.clear();

	shared_ptr<HImageFilter> AfterOM = make_shared<HImageFilter>();
	AfterOM->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	AfterOM->SetShaderResources({ m_renderTargetResourceView });
	AfterOM->SetRenderTargets({ m_renderTargetView });
	ImageFilters.push_back(AfterOM);

	/*for (int i = 0; i < 10; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> Prev = ImageFilters.back()->m_shaderResourceView;
		shared_ptr<HImageFilter> BlurX = make_shared<HImageFilter>();
		BlurX->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Blur/BlurXPixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
		BlurX->SetShaderResources({ Prev });
		ImageFilters.push_back(BlurX);


		ComPtr<ID3D11ShaderResourceView> Prev2 = ImageFilters.back()->m_shaderResourceView;
		shared_ptr<HImageFilter> BlurY = make_shared<HImageFilter>();
		BlurY->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Blur/BlurYPixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
		BlurY->SetShaderResources({ Prev2 });
		ImageFilters.push_back(BlurY);
	}
	*/


	//ComPtr<ID3D11ShaderResourceView> Prev = ImageFilters.back()->m_shaderResourceView;
	//shared_ptr<HImageFilter> FinalRendering = make_shared<HImageFilter>();	
	//FinalRendering->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	//FinalRendering->SetShaderResources({ Prev });
	//FinalRendering->SetRenderTargets({ m_renderTargetView });
	//ImageFilters.push_back(FinalRendering);


}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Update();
	}
	GUIRenderSubModule.Update();
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


	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Render();
	}

	for (size_t i = 0; i < ImageFilters.size(); ++i)
	{
		ImageFilters[i]->Render();
	}

	GUIRenderSubModule.Render();

	m_swapChain->Present(1, 0);

}
