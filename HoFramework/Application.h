#pragma once
#include <windows.h>
#include "InputModule.h"
#include "Define.h"
#include "CustomRenderModule.h"
#include <vector>
#include <functional>

class Application
{
public:
	Application();
	~Application() {};
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int Run();
	
	vector<std::function<LRESULT(HWND, UINT , WPARAM , LPARAM)>> OnMessageProc;


private:
	bool InitWindows();


private:
	//Components
	HInputModule InputModule;
	std::shared_ptr<HBaseRenderModule> RenderModule = nullptr;
	
public:
	HInputModule& GetInputModule() {
		return InputModule;
	}

	inline bool IsInitialized() {
		return m_IsInitialized;
	};
private:
	bool m_IsInitialized = false;
	
public:
	int GetScreenWidth() const { return m_screenWidth; };
	int GetScreenHeight() const { return m_screenHeight; };
	bool IsHorizontalView() const { return m_screenWidth > m_screenHeight; };
	float GetAspectRatio() const { return  (float)m_screenWidth / m_screenHeight; };

	HWND GetMainWindow() const { return m_mainWindow; };

private:
	int m_screenWidth = 1920;
	int m_screenHeight = 1080;
	HWND m_mainWindow;

};



inline Application* GApp = nullptr;
