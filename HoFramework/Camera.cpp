#include "Camera.h"
#include "Application.h"
#include <d3d11.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

Matrix HCamera::GetViewMatrix()
{
	//TODO : Translate 할 때, 움직이는것 자체는 뒤집어 줘야함. 뷰 행렬을 이용하여 렌더링을 할 때 카메라가 오브젝트에 다가가는게 아닌 오브젝트가 카메라쪽으로 다가오는 메커니즘으로 그리게 된다.
	// 왜냐면 이 행렬은 오브젝트를 그리는 쉐이더에서 사용하기 때문.
	Matrix Final = Matrix::CreateTranslation(-GlobalCameraPosition) * Matrix::CreateRotationY(-GlobalCameraRotation.y) * Matrix::CreateRotationX(GlobalCameraRotation.x);
	return Final.Transpose();
}

Matrix HCamera::GetProjectionMatrix()
{
	Matrix Result;
	if (m_IsPersepective)
	{

		const float usingFOVAngle = m_FOVInDeg * (XM_PI / 180.f);
		Result = XMMatrixPerspectiveFovLH(usingFOVAngle, m_ParentApp->GetAspectRatio(), 0.01f, 100.0f);

	}
	else
	{
		Result = XMMatrixOrthographicLH(m_ParentApp->GetAspectRatio(), 0.f, 0.01f, 100.0f);
	}

	return Result.Transpose();
}