#include "ImGUIRenderSubModule.h"
#include "BaseRenderModule.h"
#include "Application.h"
#include "FBXRenderingObject.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

HImGUIRenderSubModule::~HImGUIRenderSubModule()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool HImGUIRenderSubModule::Initialize(HBaseRenderModule* InParentRenderModule)
{
	m_ParentRenderModule = InParentRenderModule;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2(float(m_ParentRenderModule->GetAppContext()->GetScreenWidth()), float(m_ParentRenderModule->GetAppContext()->GetScreenHeight()));
	//색상 지정
	ImGui::StyleColorsDark();

	if (!ImGui_ImplDX11_Init(m_ParentRenderModule->GetDevice().Get(), m_ParentRenderModule->GetContext().Get())) {
		return false;
	}

	if (!ImGui_ImplWin32_Init(m_ParentRenderModule->GetAppContext()->GetMainWindow())) {
		return false;
	}

	std::function<LRESULT(HWND, UINT, WPARAM, LPARAM)> Callback = &OnGUIMsgProc;
	m_ParentRenderModule->GetAppContext()->OnMessageProc.push_back(Callback);

	return true;
}


LRESULT OnGUIMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;
}

void HImGUIRenderSubModule::Update()
{
	//값 업데이트

}

void HImGUIRenderSubModule::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::Begin("Scene Control");
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	//ImGui::SliderFloat3("translation", &m_ParentRenderModule->GetGlobalCameraPosition().x, -1.f, 1.f);
	//ImGui::SliderFloat3("rotation", &m_ParentRenderModule->GetGlobalCameraRotation().x,-180.f * 3.141592/180.f, 180.f * 3.141592 / 180.f);
	//ImGui::SliderFloat("RimIntensity", &((HFBXDragonRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSExtraData.RimIntensity,0.f, 10.f);
	//ImGui::SliderFloat("RimPow", &((HFBXDragonRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSExtraData.RimPow, 0.f, 10.f);


	ImGui::SliderFloat3("Directional Light Light Pos ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].LightPos.x, -1.f, 1.f);
	ImGui::SliderFloat("Directional Light LightIntensity ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].LightIntensity, 0.f, 10.f);
	ImGui::SliderFloat3("Directional Light Light Dir ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].LightDir.x, -1.f, 1.f);
	ImGui::SliderFloat("Directional Light Falloff Start ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].FalloffStart, 0.f, 5.f);
	ImGui::SliderFloat("Directional Light Falloff End ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].FalloffEnd, 0.f, 5.f);
	ImGui::SliderFloat("Directional Light SpotFactor ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[0].SpotFactor, 0.f, 5.f);

	ImGui::SliderFloat3("Point Light Light Pos ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].LightPos.x, -1.f, 1.f);
	ImGui::SliderFloat("Point Light LightIntensity ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].LightIntensity, 0.f, 10.f);
	ImGui::SliderFloat3("Point Light Light Dir ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].LightDir.x, -1.f, 1.f);
	ImGui::SliderFloat("Point Light Falloff Start ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].FalloffStart, 0.f, 5.f);
	ImGui::SliderFloat("Point Light Falloff End ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].FalloffEnd, 0.f, 2000.f);
	ImGui::SliderFloat("Point Light Spot Factor ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[1].SpotFactor, 0.f, 5.f);

	ImGui::SliderFloat("Spot Light LightIntensity ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].LightIntensity, 0.f, 10.f);
	ImGui::SliderFloat3("Spot Light Light Pos ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].LightPos.x, -1.f, 1.f);
	ImGui::SliderFloat3("Spot Light Light Dir ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].LightDir.x, -1.f, 1.f);
	ImGui::SliderFloat("Spot Light Falloff Start ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].FalloffStart, 0.f, 5.f);
	ImGui::SliderFloat("Spot Light Falloff End ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].FalloffEnd, 0.f, 2000.f);
	ImGui::SliderFloat("Spot Light Spot Factor ", &((HFBXRenderingObject*)(m_ParentRenderModule->GetRenderingObjects()[0].get()))->m_PSConstBufferData.UsingLight[2].SpotFactor, 0.f, 5.f);

	ImGui::Checkbox("Wireframe", &m_ParentRenderModule->GetIsWireframe());

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
