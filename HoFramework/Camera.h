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
		GlobalCameraRotation = InRotateEuler;

		ForwardVector = Vector3::Transform(Vector3(0,0,1), Matrix::CreateRotationY(GlobalCameraRotation.y));
		//UpVector = Vector3::Transform(Vector3(0, 1, 0), Matrix::CreateRotationX(GlobalCameraRotation.x));
		RightVector = UpVector.Cross(ForwardVector);
	}
	inline void Translate(Vector3 InTranslate)
	{	
		//TODO : Translate �� ��, �����̴°� ��ü�� ������ �����. �� ����� �̿��Ͽ� �������� �� �� ī�޶� ������Ʈ�� �ٰ����°� �ƴ� ������Ʈ�� ī�޶������� �ٰ����� ��Ŀ�������� �׸��� �ȴ�.
		// �ֳĸ� �� ����� ������Ʈ�� �׸��� ���̴����� ����ϱ� ����.
		GlobalCameraPosition += InTranslate;
	}

	Matrix GetViewMatrix();
	Matrix GetProjectionMatrix();

	Vector3 GetForward() { return ForwardVector; };
	Vector3 GetRight() { return RightVector; };
	Vector3 GetUp() { return UpVector; };
	

protected:
	bool m_IsPersepective = true;
	float m_FOVInDeg = 70.f;

	class Application* m_ParentApp = nullptr;

	Vector3 GlobalCameraPosition = Vector3(0.f, 0.f, 2.f);
	Vector3 GlobalCameraRotation = Vector3(0.f, 0.f, 0.f);

	//TODO: ȸ�� �� ���� ���� ���� ��ȯ �ǰԲ� ���� �ʿ�
	Vector3 RightVector = Vector3(1.f, 0.f, 0.f);
	Vector3 UpVector = Vector3(0.f, 1.f, 0.f);
	Vector3 ForwardVector = Vector3(0.f, 0.f, 1.f);
};