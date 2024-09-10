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

	if (ImGui::TreeNode("Lights")) {
		if (ImGui::TreeNode("Directional Light")) {
			ImGui::SliderFloat("Directional Light LightIntensity ", &m_ParentRenderModule->m_LightPSConstant.Lights[0].LightIntensity, 0.f, 10.f);
			ImGui::SliderFloat3("Directional Light Light Dir ", &m_ParentRenderModule->m_LightPSConstant.Lights[0].LightDir.x, -1.f, 1.f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Point Light")) {
			ImGui::SliderFloat3("Point Light Light Pos ", &m_ParentRenderModule->m_LightPSConstant.Lights[1].LightPos.x, -1.f, 1.f);
			ImGui::SliderFloat("Point Light LightIntensity ", &m_ParentRenderModule->m_LightPSConstant.Lights[1].LightIntensity, 0.f, 10.f);
			ImGui::SliderFloat("Point Light Falloff Start ", &m_ParentRenderModule->m_LightPSConstant.Lights[1].FalloffStart, 0.f, 5.f);
			ImGui::SliderFloat("Point Light Falloff End ", &m_ParentRenderModule->m_LightPSConstant.Lights[1].FalloffEnd, 0.f, 20.f);
			ImGui::SliderFloat("Point Light Spot Factor ", &m_ParentRenderModule->m_LightPSConstant.Lights[1].SpotFactor, 0.f, 5.f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spot Light")) {
			ImGui::SliderFloat("Spot Light LightIntensity ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].LightIntensity, 0.f, 10.f);
			ImGui::SliderFloat3("Spot Light Light Pos ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].LightPos.x, -1.f, 1.f);
			ImGui::SliderFloat3("Spot Light Light Dir ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].LightDir.x, -1.f, 1.f);
			ImGui::SliderFloat("Spot Light Falloff Start ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].FalloffStart, 0.f, 5.f);
			ImGui::SliderFloat("Spot Light Falloff End ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].FalloffEnd, 0.f, 20.f);
			ImGui::SliderFloat("Spot Light Spot Factor ", &m_ParentRenderModule->m_LightPSConstant.Lights[2].SpotFactor, 0.f, 5.f);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}


	//if (ImGui::TreeNode("Zelda Object")) {

	//	std::shared_ptr<HBaseRenderingObject> RenderingObject = m_ParentRenderModule->GetRenderingObjects()[0];

	//	vector<MeshObject> Meshes =  RenderingObject->GetDrawingMeshes();
	//		
	//	

	//	for (int i = 0; i < Meshes.size(); ++i)
	//	{
	//		string name = "Zelda Object : " + i;
	//		if (ImGui::TreeNode(name.c_str()))
	//		{
	//			Material& MatInfo = Meshes[i].materialPSConstantData.Mat;
	//			ImGui::SliderFloat("Ambient Strength", &MatInfo.AmbientStrength, 0.f, 10.f);
	//			ImGui::SliderFloat3("FresnelR0", &MatInfo.FresnelR0.x, 0.f, 1.f);

	//			//ImGui::SliderInt("Shading Model", &MatInfo.ShadingModel, 0, 1);
	//			ImGui::Checkbox("Shading Model", &MatInfo.UsePBR);
	//			ImGui::SliderFloat("Shiness", &MatInfo.Shiness, 0, 10);
	//			ImGui::Checkbox("NormalMap", &MatInfo.UseNormalMap);
	//			ImGui::Checkbox("MetallicMap", &MatInfo.UseMetallicMap);
	//			ImGui::Checkbox("RoughnessMap", &MatInfo.UseRoughnessMap);
	//			ImGui::TreePop();
	//		}
	//	}


	//	ImGui::TreePop();
	//}


	if (ImGui::TreeNode("Sphere Object")) {

		std::shared_ptr<HBaseRenderingObject> RenderingObject = m_ParentRenderModule->GetRenderingObjects()[0];
		Material& MatInfo = RenderingObject->GetDrawingMeshes()[0].materialPSConstantData.Mat;

		ImGui::SliderFloat("Ambient Strength", &MatInfo.AmbientStrength, 0.f, 10.f);
		ImGui::SliderFloat("Roughness", &MatInfo.Roughness, 0.f, 10.f);
		ImGui::SliderFloat("Metalic", &MatInfo.Metalic, 0.f, 10.f);
		ImGui::SliderFloat("Shiness", &MatInfo.Shiness, 0, 10);
		ImGui::SliderFloat4("FresnelR0", &MatInfo.FresnelR0.x, 0.f, 1.f);
		ImGui::SliderFloat4("Specular", &MatInfo.Specular.x, 0.f, 1.f);

		//ImGui::SliderInt("Shading Model", &MatInfo.ShadingModel, 0, 1);

		ImGui::SliderInt("AlbedoMap", &MatInfo.UseAlbedoMap,0,1);
		ImGui::SliderInt("NormalMap", &MatInfo.UseNormalMap, 0, 1);
		ImGui::SliderInt("MetallicMap", &MatInfo.UseMetallicMap, 0, 1);
		ImGui::SliderInt("RoughnessMap", &MatInfo.UseRoughnessMap, 0, 1);
		ImGui::SliderInt("UseIBL", &MatInfo.UseIBL, 0, 1);
		ImGui::SliderInt("UsePBR", &MatInfo.UsePBR, 0, 1);

		ImGui::TreePop();
	}


	if (ImGui::TreeNode("Misc")) {
		float* gamma = m_ParentRenderModule->GetGamma();
		float* exposure = m_ParentRenderModule->GetExposure();
		ImGui::SliderFloat("Gamma ", gamma, 0.f, 10.f);
		ImGui::SliderFloat("Exposure ", exposure, 0.f, 10.f);

		ImGui::Checkbox("Wireframe", &m_ParentRenderModule->GetIsWireframe());


		ImGui::TreePop();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
