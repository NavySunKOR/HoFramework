#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "FBXRenderingObject.h"
#include "SkyBoxRenderingObject.h"
#include "DefaultRenderingObject.h"
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

	//TODO: 에디터처럼 만들려면 나중에 이걸 외부에서 추가 할 수 있도록 변경 필요.

	//라이트 설정

	HRenderingLibrary::CreateConstantBuffer<LightingPSConstantBuffer>(GetDevice(), m_LightPSConstant, m_LightPSConstantBuffer);

	SkyBoxObject = std::make_shared<HSkyBoxRenderingObject>(this);

	Mesh mesh;
	HRenderingLibrary::MakeBox(&mesh, 20.f);
	SkyBoxObject->ApplyMesh(mesh);
	SkyBoxObject->SetSkyboxResources(L"./skybox/clearSky/CloudCommon_diffuseIBL.dds", L"./skybox/clearSky/CloudCommons_specularIBL.dds");
	SkyBoxObject->SetVertexShader(L"./Shaders/Skybox/VertexShaderSkybox.hlsl", "main");
	SkyBoxObject->SetPixelShader(L"./Shaders/Skybox/PixelShaderSkybox.hlsl", "main");


	std::shared_ptr<HFBXRenderingObject> ZeldaObject = std::make_shared<HFBXRenderingObject>(this);
	ZeldaObject->ApplyMesh("./Meshes/zeldaPosed001/", "zeldaPosed001.fbx");
	ZeldaObject->SetVertexShader(L"./Shaders/Lit/LitVertexShader.hlsl", "main");
	ZeldaObject->SetPixelShader(L"./Shaders/Lit/LitPixelShader.hlsl", "main");
	ZeldaObject->Scale(Vector3(0.01f, 0.01f, 0.01f));
	ZeldaObject->Translate(Vector3(0.f, -1.f, 0.f));


	std::shared_ptr<HDefaultRenderingObject> SphereObject = std::make_shared<HDefaultRenderingObject>(this);

	std::vector<string> Resources;
	Resources.reserve(2);
	Resources.push_back(string("./SampleTexture/brick-wall/brick-wall_albedo.png"));
	Resources.push_back(string("./SampleTexture/brick-wall/brick-wall_normal-dx.png"));
	/*Resources.push_back(string("./skybox/clearSky/CloudCommon_diffuseIBL.dds"));
	Resources.push_back(string("./skybox/clearSky/CloudCommons_specularIBL.dds"));*/

	SphereObject->SetExternalResource(Resources);
	SphereObject->ApplyMesh(EPrimitiveType::Sphere);
	SphereObject->SetVertexShader(L"./Shaders/Lit/LitVertexShader.hlsl", "main");
	SphereObject->SetPixelShader(L"./Shaders/Lit/LitPixelShaderVer2.hlsl", "main");
	SphereObject->Scale(Vector3(1.f, 1.f, 1.f));
	SphereObject->Translate(Vector3(0.f, -1.f, -2.f));


	RenderingObjects.reserve(2);
	RenderingObjects.push_back(ZeldaObject);
	RenderingObjects.push_back(SphereObject);


	SkyBoxObject->Initialize();

	m_LightPSConstant.Lights[0].LightIntensity = 2.f;

	for (size_t i = 0; i < RenderingObjects.size(); ++i)
	{
		RenderingObjects[i]->Initialize();
	}

	//InitImageFilters();

	if (!GUIRenderSubModule.Initialize(this))
		return true;

	return true;
}


void HCustomRenderModule::InitImageFilters()
{
	ImageFilters.clear();

	//Output Merger 이후 완성된 백버퍼
	shared_ptr<HImageFilter> AfterOM = make_shared<HImageFilter>();
	AfterOM->Initialize(this, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight());
	AfterOM->SetShaderResources({ m_renderTargetResourceView });
	ImageFilters.push_back(AfterOM);


	//InitBlurFilter();



	//필터 처리 된 화면을 다시 렌더 타겟 뷰 에 적용
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



void HCustomRenderModule::UpdateInput()
{
	//std::shared_ptr<HFBXRenderingObject> Zelda = (std::shared_ptr<HFBXRenderingObject>)(RenderingObjects[0]);
	HInputModule Input = m_AppContext->GetInputModule();
	const int KeyW = 0x57;
	const int KeyS = 0x53;
	const int KeyA = 0x41;
	const int KeyD = 0x44;
	const int KeyF = 0x46;



	if (Input.GetKey(KeyF))
		m_MoveFPP = !m_MoveFPP;

	if(Input.GetKey(KeyW) && m_MoveFPP)
		m_MainView.Translate(m_MainView.GetForward() * 0.016f * 10.f);

	if (Input.GetKey(KeyS) && m_MoveFPP)
		m_MainView.Translate(m_MainView.GetForward() * -1.f * 0.016f * 10.f);

	if (Input.GetKey(KeyA) && m_MoveFPP)
		m_MainView.Translate(m_MainView.GetRight() * -1.f * 0.016f * 10.f);

	if (Input.GetKey(KeyD) && m_MoveFPP)
		m_MainView.Translate(m_MainView.GetRight() * 0.016f * 10.f);



	if (m_MoveFPP)
	{
		Vector2 Mouse = m_AppContext->GetInputModule().GetMousePositionNDC();
		//Mouse.y * (DirectX::XM_PI / 2)
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
