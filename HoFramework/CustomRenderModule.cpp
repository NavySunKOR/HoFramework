#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "FBXRenderingObject.h"
#include "SkyBoxRenderingObject.h"
#include "DefaultRenderingObject.h"
#include "ImageFilter.h"
#include "SeaImageFilter.h"
#include "GraphicsCommon.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//Display Setting
	Gamma = 0.5f;
	Exposure = 1.f;

	//Light setting

	m_Lights[1].ConstantData.LightIntensity = 2.f;


	HRenderingLibrary::CreateConstantBuffer<LightingPSConstantBuffer>(GetDevice(), m_LightPSConstant, m_LightPSConstantBuffer);


	//Mesh setting
	SkyBoxObject = std::make_shared<HSkyBoxRenderingObject>(this);

	SkyBoxObject->ApplyMesh(EPrimitiveType::Sphere);
	SkyBoxObject->SetSkyboxResources(L"./skybox/ForestWall/ForestWallEnvHDR.dds",L"./skybox/ForestWall/ForestWallDiffuseHDR.dds", L"./skybox/ForestWall/ForestWallSpecularHDR.dds", L"./skybox/ForestWall/ForestWallBrdf.dds");
	SkyBoxObject->Scale(Vector3(10.f, 10.f, 10.f));


	SkyBoxObject->Initialize();

	std::shared_ptr<HDefaultRenderingObject> SphereObject = std::make_shared<HDefaultRenderingObject>(this);
	SphereObject->ApplyMesh(EPrimitiveType::Sphere);
	SphereObject->SetExternalResource(0, EModelTexture2DType::Albedo, string("./SampleTexture/brick-wall/brick-wall_albedo.png"));
	SphereObject->SetExternalResource(0, EModelTexture2DType::Normal, string("./SampleTexture/brick-wall/brick-wall_normal-dx.png"));
	SphereObject->SetExternalResource(0, EModelTexture2DType::Metallic, string("./SampleTexture/brick-wall/brick-wall_metallic.png"));
	SphereObject->SetExternalResource(0, EModelTexture2DType::Roughness, string("./SampleTexture/brick-wall/brick-wall_roughness.png"));
	SphereObject->SetSkyboxSRVs(SkyBoxObject->GetSkyboxDiffuse(), SkyBoxObject->GetSkyboxSpecular(),SkyBoxObject->GetSkyboxBRDF());
	SphereObject->Scale(Vector3(1.f, 1.f, 1.f));
	SphereObject->Translate(Vector3(0.f, -1.f, -2.f));

	RenderingObjects.reserve(1);
	RenderingObjects.push_back(SphereObject);

	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Initialize();
	}


	//Post Process seting
	InitImageFilters();


	//UI Init
	if (!GUIRenderSubModule.Initialize(this))
		return true;

	return true;
}


void HCustomRenderModule::InitImageFilters()
{
	ImageFilters.clear();

	//Output Merger 이후 완성된 백버퍼
	shared_ptr<HImageFilter> AfterOM = make_shared<HImageFilter>();
	AfterOM->Initialize(this,m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	AfterOM->SetShaderResources({ m_renderTargetResourceView });
	ImageFilters.push_back(AfterOM);

	//필터 처리 된 화면을 다시 렌더 타겟 뷰 에 적용
	ComPtr<ID3D11ShaderResourceView> Prev = ImageFilters.back()->m_shaderResourceView;
	shared_ptr<HImageFilter> FinalRendering = make_shared<HImageFilter>();	
	FinalRendering->Initialize(this, m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	FinalRendering->SetShaderResources({ Prev });
	FinalRendering->SetRenderTargets({ m_renderTargetView });
	ImageFilters.push_back(FinalRendering);

}


void HCustomRenderModule::InitBlurFilter()
{

	for (int i = 0; i < 10; ++i)
	{
		ComPtr<ID3D11ShaderResourceView> Prev = ImageFilters.back()->m_shaderResourceView;
		shared_ptr<HImageFilter> BlurX = make_shared<HImageFilter>();
		BlurX->Initialize(this, m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
		BlurX->SetShaderResources({ Prev });
		ImageFilters.push_back(BlurX);


		ComPtr<ID3D11ShaderResourceView> Prev2 = ImageFilters.back()->m_shaderResourceView;
		shared_ptr<HImageFilter> BlurY = make_shared<HImageFilter>();
		BlurY->Initialize(this,  m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
		BlurY->SetShaderResources({ Prev2 });
		ImageFilters.push_back(BlurY);
	}
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
	UpdateInput();
	
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

	HRenderingLibrary::UpdateConstantBuffer(m_LightPSConstant, m_LightPSConstantBuffer, GetContext());
	GUIRenderSubModule.Update();
}


void HCustomRenderModule::RenderShadowDepths()
{
	for (int i = 0; i < LIGHT_COUNTS; ++i)
	{

		
		SetViewport(&Graphics::Defines::shadowViewport);

		//SetPSO(Graphics::States::shadowDepth);

		for (size_t i = 0; i < RenderingObjects.size(); ++i)
		{
			RenderingObjects[i]->Render(m_Lights[i].LightView);
		}
	}
}

void HCustomRenderModule::RenderFinalColor()
{
	SetViewport(&Graphics::Defines::screenViewport);

	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//이거 매 프레임마다 클리어 해줄 것.
	if (m_depthStencilView)
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	else
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);


	SetPSO(Graphics::States::skyboxPSO);
	if (SkyBoxObject)
		SkyBoxObject->Render(m_MainView);


	SetPSO((GetIsWireframe()) ? Graphics::States::basicWirePSO : Graphics::States::basicSolidPSO);
	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Render(m_MainView);
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	m_context->ResolveSubresource(m_tempTexture.Get(), 0, backBuffer.Get(), 0,
		DXGI_FORMAT_R16G16B16A16_FLOAT);


	//TODO: 여기에 Blur나 Bloom 섞으면 조절필요.
	SetPSO(Graphics::States::postProcessBasePSO);
	for (size_t i = 0; i < ImageFilters.size(); ++i)
	{
		ImageFilters[i]->Render();
	}

	GUIRenderSubModule.Render();

	m_swapChain->Present(1, 0);
}

void HCustomRenderModule::UpdateInput()
{
	HInputModule Input = m_AppContext->GetInputModule();
	const int KeyW = 0x57;
	const int KeyS = 0x53;
	const int KeyA = 0x41;
	const int KeyD = 0x44;
	const int KeyF = 0x46;



	if (Input.GetKeyDown(KeyF))
	{
		m_MoveFPP = !m_MoveFPP;
	}

	if (m_MoveFPP)
	{
		if (Input.GetKey(KeyW))
			m_MainView.Translate(m_MainView.GetForward() * 0.016f * 10.f);

		if (Input.GetKey(KeyS))
			m_MainView.Translate(m_MainView.GetForward() * -1.f * 0.016f * 10.f);

		if (Input.GetKey(KeyA))
			m_MainView.Translate(m_MainView.GetRight() * -1.f * 0.016f * 10.f);

		if (Input.GetKey(KeyD))
			m_MainView.Translate(m_MainView.GetRight() * 0.016f * 10.f);

		Vector2 Mouse = m_AppContext->GetInputModule().GetMousePositionNDC();
		m_MainView.Rotate(Vector3(Mouse.y * (DirectX::XM_PIDIV2), Mouse.x * (DirectX::XM_2PI), 0));
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

	RenderFinalColor();
}
