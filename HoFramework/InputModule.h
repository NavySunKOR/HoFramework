#pragma once
#include <windows.h>
#include "Define.h"


//키코드 무조건 지정
enum EMouseButton
{
	Left = VK_LBUTTON,
	Right = VK_RBUTTON
};

class HInputModule
{
public:

	void OnKeyDown(WPARAM pKeyCode);
	void OnKeyUp(WPARAM pKeyCode);
	void OnMouseMove(LPARAM lParam);
	bool GetKey(WPARAM pKeyCode) const
	{
		return Keys[(int)pKeyCode];
	}

	inline Vector2 GetMousePositionNDC() const
	{
		return m_MouseNDCPos;
	};
private:
	Vector2 m_MouseNDCPos;
	bool Keys[255] = { false };
};