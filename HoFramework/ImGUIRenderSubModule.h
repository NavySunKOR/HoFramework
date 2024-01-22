#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <windows.h>
class HBaseRenderModule;

LRESULT OnGUIMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class HImGUIRenderSubModule
{

public:
	~HImGUIRenderSubModule();
	bool Initialize(HBaseRenderModule* InParentRenderModule);
	void Update();
	void Render();

private:


	HBaseRenderModule* m_ParentRenderModule;
};