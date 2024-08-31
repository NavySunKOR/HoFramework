#pragma once
#include "Define.h"
#include <d3d11.h>
#include <wrl.h>

class HCamera
{
public:

	inline void SetApplication(class Application* InApp)
	{
		m_ParentApp = InApp;
	}

	inline void Rotate(Vector3 InRotateEuler)
	{
		GlobalCameraRotation += InRotateEuler;
	}
	inline void Translate(Vector3 InTranslate)
	{
		GlobalCameraPosition += InTranslate;
	}

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

protected:
	bool m_IsPersepective = true;
	float m_FOVInDeg = 70.f;

	class Application* m_ParentApp = nullptr;

	Vector3 GlobalCameraPosition = Vector3(0.f, 0.f, 2.f);
	Vector3 GlobalCameraRotation = Vector3(0.f, 0.f, 0.f);

	//TODO: 회전 시 방향 벡터 또한 전환 되게끔 변경 필요
	Vector3 GlobalCameraRightVector = Vector3(1.f, 0.f, 0.f);
	Vector3 GlobalCameraUpVector = Vector3(0.f, 1.f, 0.f);
	Vector3 GlobalCameraForward = Vector3(0.f, 0.f, 1.f);
};