#pragma once
#include <windows.h>
#include "InputModule.h"
#include "RenderModule.h"
#include "Define.h"


class Application
{
public:
	Application();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Run();
	
private:
	bool InitWindows();


private:
	//Components
	HInputModule InputModule;
	HBaseRenderModule RenderModule;
	
public:
	inline bool IsInitialized() {
		return m_IsInitialized;
	};
private:
	bool m_IsInitialized = false;
	
public:
	int GetScreenWidth() const { return m_screenWidth; };
	int GetScreenHeight() const { return m_screenHeight; };

	HWND GetMainWindow() const { return m_mainWindow; };

private:
	int m_screenWidth = 1920;
	int m_screenHeight = 1080;
	HWND m_mainWindow;

};
