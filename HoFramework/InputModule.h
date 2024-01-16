#pragma once
#include <windows.h>

enum EMouseButton
{
	Left,
	Right
};

class HInputModule
{
public:
	void OnKeyDown(WPARAM pKeyCode);
	void OnMouseMove(LPARAM lParam);
	void OnMouseButtonDown(EMouseButton pMouseButton);
};