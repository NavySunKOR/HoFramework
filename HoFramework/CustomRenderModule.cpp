#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "FBXRenderingObject.h"
#include "SkyBoxRenderingObject.h"
#include "ImageFilter.h"
#include "SeaImageFilter.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//TODO: ������ó�� ������� ���߿� �̰� �ܺο��� �߰� �� �� �ֵ��� ���� �ʿ�.

	SkyBoxObject = std::make_shared<HSkyBoxRenderingObject>(this);

	Mesh mesh;
	HRenderingLibrary::MakeBox(&mesh, 20.f);
	SkyBoxObject->ApplyMesh(mesh);
	SkyBoxObject->SetSkyboxResources(L"./skybox/clearSky/CloudCommon_diffuseIBL.dds", L"./skybox/clearSky/CloudCommons_specularIBL.dds");
	SkyBoxObject->SetVertexShader(L"./Shaders/Skybox/VertexShaderSkybox.hlsl", "main");
	SkyBoxObject->SetPixelShader(L"./Shaders/Skybox/PixelShaderSkybox.hlsl", "main");


	std::shared_ptr<HFBXRenderingObject> ZeldaObject = std::make_shared<HFBXRenderingObject>(this);
	ZeldaObject->Scale(Vector3(0.01f, 0.01f, 0.01f));
	ZeldaObject->Translate(Vector3(0.f, -1.f, 0.f));


	RenderingObjects.reserve(1);
	RenderingObjects.push_back(ZeldaObject);


	SkyBoxObject->Initialize();

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

	//Output Merger ���� �ϼ��� �����
	shared_ptr<HImageFilter> AfterOM = make_shared<HImageFilter>();
	AfterOM->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	AfterOM->SetShaderResources({ m_renderTargetResourceView });
	ImageFilters.push_back(AfterOM);


	//InitBlurFilter();



	//���� ó�� �� ȭ���� �ٽ� ���� Ÿ�� �� �� ����
	ComPtr<ID3D11ShaderResourceView> Prev = ImageFilters.back()->m_shaderResourceView;
	shared_ptr<HImageFilter> FinalRendering = make_shared<HImageFilter>();	
	FinalRendering->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	FinalRendering->SetShaderResources({ Prev });
	FinalRendering->SetRenderTargets({ m_renderTargetView });
	ImageFilters.push_back(FinalRendering);


	//shared_ptr<HSeaImageFilter> FinalRendering = make_shared<HSeaImageFilter>();
	//FinalRendering->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/ShaderToy/SeaPixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	//FinalRendering->SetShaderResources({ m_renderTargetResourceView });
	//FinalRendering->SetRenderTargets({ m_renderTargetView });
	//ImageFilters.push_back(FinalRendering);


}


void HCustomRenderModule::InitBlurFilter()
{

	for (int i = 0; i < 10; ++i)
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
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//���⼭���� �ۼ� 
	
	if (SkyBoxObject)
		SkyBoxObject->Update();

	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Update();
	}

	for (size_t i = 0; i < ImageFilters.size(); ++i)
	{
		ImageFilters[i]->Update();
	}
	GUIRenderSubModule.Update();
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

	if(SkyBoxObject)
	SkyBoxObject->Render();

	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Render();
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	m_context->ResolveSubresource(m_tempTexture.Get(), 0, backBuffer.Get(), 0,
		DXGI_FORMAT_R8G8B8A8_UNORM);

	for (size_t i = 0; i < ImageFilters.size(); ++i)
	{
		ImageFilters[i]->Render();
	}

	GUIRenderSubModule.Render();

	m_swapChain->Present(1, 0);

}
