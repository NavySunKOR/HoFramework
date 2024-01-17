#include "CustomRenderModule.h"
#include "Application.h"

void HCustomRenderModule::UpdateCube1()
{
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;

	m_transformConstData.ModelTransform = (Matrix::CreateScale(0.25f) * Matrix::CreateRotationY(RotationYValue) * Matrix::CreateRotationX(RotationXValue)).Transpose();

	using namespace DirectX;
	m_transformConstData.ViewTransform = XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();

	const float fovAngle = ViewAngleInDeg * (XM_PI / 180.f);
	m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(fovAngle, (float)m_AppContext->GetScreenWidth() / m_AppContext->GetScreenHeight(), 0.01f, 100.0f);
	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();

	D3D11_MAPPED_SUBRESOURCE ms;
	m_context->Map(m_transformConstBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &m_transformConstData, sizeof(m_transformConstData));
	m_context->Unmap(m_transformConstBuffer.Get(), NULL);
}

void HCustomRenderModule::UpdateCube2()
{

}