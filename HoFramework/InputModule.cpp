#include "InputModule.h"
#include "Application.h"
#include <algorithm>

void HInputModule::OnKeyDown(WPARAM pKeyCode)
{
	Keys[(int)pKeyCode] = true;
}

void HInputModule::OnKeyUp(WPARAM pKeyCode)
{
	Keys[(int)pKeyCode] = false;
}

void HInputModule::OnMouseMove(LPARAM lParam)
{
	//GApp
	int width = GApp->GetScreenWidth();
	int height = GApp->GetScreenHeight();
	int mouseX = LOWORD(lParam);
	int mouseY = HIWORD(lParam);
	

	float x = ((float)mouseX / width) * 2.f - 1;
	float y = -(((float)mouseY / height) * 2.f - 1);

	x = std::clamp(x, -1.f, 1.f);
	y = std::clamp(y, -1.f, 1.f);

	m_MouseNDCPos.x = x;
	m_MouseNDCPos.y = y;
}
