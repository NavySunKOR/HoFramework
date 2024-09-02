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
		//TODO : Translate 할 때, 움직이는것 자체는 뒤집어 줘야함. 뷰 행렬을 이용하여 렌더링을 할 때 카메라가 오브젝트에 다가가는게 아닌 오브젝트가 카메라쪽으로 다가오는 메커니즘으로 그리게 된다.
		// 왜냐면 이 행렬은 오브젝트를 그리는 쉐이더에서 사용하기 때문.
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

	//TODO: 회전 시 방향 벡터 또한 전환 되게끔 변경 필요
	Vector3 RightVector = Vector3(1.f, 0.f, 0.f);
	Vector3 UpVector = Vector3(0.f, 1.f, 0.f);
	Vector3 ForwardVector = Vector3(0.f, 0.f, 1.f);
};