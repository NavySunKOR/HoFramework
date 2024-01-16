#pragma once
#include <windows.h>
#include "InputModule.h"


class Application
{
public:
	Application();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	inline bool IsInitialized() {
		return m_IsInitialized;
	};
	int Run();
	
private:
	bool InitWindows();


private:
	//Components

	HInputModule InputModule;
	
	
	
	
	bool m_IsInitialized = false;
	HWND m_mainWindow;
	int m_screenWidth = 1920;
	int m_screenHeight = 1080;

};
