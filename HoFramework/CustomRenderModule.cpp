#include "CustomRenderModule.h"
#include "Application.h"
#include "RenderingLibrary.h"
#include "LoadFBXRenderingObject.h"
#include "FBXDragonRenderingObject.h"
#include "CubeMapRenderingObject.h"
#include "EnvMapSphereRenderingObject.h"

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

	if (!GUIRenderSubModule.Initialize(this))
		return true;

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

	GUIRenderSubModule.Render();
	m_swapChain->Present(1, 0);

}
