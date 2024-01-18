#include "Application.h"
#include <iostream>

Application* GApp = nullptr;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    // g_appBase�� �̿��ؼ� ���������� ��� �Լ� ȣ��
    return GApp->MsgProc(hWnd, msg, wParam, lParam);
}


bool Application::InitWindows()
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX),
                   CS_CLASSDC,
                   WndProc,
                   0L,
                   0L,
                   GetModuleHandle(NULL),
                   NULL,
                   NULL,
                   NULL,
                   NULL,
                   L"HoFramework", // lpszClassName, L-string
                   NULL };

    // The RegisterClass function has been superseded by the RegisterClassEx
    // function.
    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa?redirectedfrom=MSDN
    if (!RegisterClassEx(&wc)) {
        std::cout << "RegisterClassEx() failed." << std::endl;
        return false;
    }

    // �츮�� ���ϴ� �׸��� �׷��� �κ��� �ػ�
    RECT wr = { 0, 0, m_screenWidth, m_screenHeight };

    // �ʿ��� ������ ũ��(�ػ�) ���
    // wr�� ���� �ٲ�
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

    // �����츦 ���鶧 ������ ����� wr ���
    m_mainWindow = CreateWindow(wc.lpszClassName, L"HoFramework",
        WS_OVERLAPPEDWINDOW,
        100, // ������ ���� ����� x ��ǥ
        100, // ������ ���� ����� y ��ǥ
        wr.right - wr.left, // ������ ���� ���� �ػ�
        wr.bottom - wr.top, // ������ ���� ���� �ػ�
        NULL, NULL, wc.hInstance, NULL);

    if (!m_mainWindow) {
        std::cout << "CreateWindow() failed." << std::endl;
        return false;
    }

    ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
    UpdateWindow(m_mainWindow);

    return true;
}

Application::Application()
{
    GApp = this;

	if (InitWindows() == false)
		return;

    RenderModule = std::make_shared<HCustomRenderModule>();
    if (RenderModule->Initialize(this) == false)
        return;

	m_IsInitialized = true;
}

LRESULT Application::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_SIZE:
            m_screenWidth = int(LOWORD(lParam));
            m_screenHeight = int(HIWORD(lParam));
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) 
            return 0;
        break;
    case WM_MOUSEMOVE:
        InputModule.OnMouseMove(lParam);
        break;
    case WM_LBUTTONUP:
        InputModule.OnMouseButtonDown(EMouseButton::Left);
        break;
    case WM_RBUTTONUP:
        // cout << "WM_RBUTTONUP Right mouse button" << endl;

        InputModule.OnMouseButtonDown(EMouseButton::Right);
        break;
    case WM_KEYDOWN:
        // cout << "WM_KEYDOWN " << (int)wParam << endl;
        InputModule.OnKeyDown(wParam);
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

int Application::Run()
{
    MSG msg = { 0 };
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            RenderModule->Run();
        }
    }

	return 0;
}
