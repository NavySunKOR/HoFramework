#include "Camera.h"
#include "Application.h"
#include <d3d11.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

Matrix HCamera::GetViewMatrix()
{
	Matrix Final = Matrix::CreateRotationX(GlobalCameraRotation.x) * Matrix::CreateRotationY(GlobalCameraRotation.y) * Matrix::CreateTranslation(GlobalCameraPosition);
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