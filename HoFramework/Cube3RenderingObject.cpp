#include "Cube3RenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include "Application.h"
#include <algorithm>


void HCube3RenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	HRenderingLibrary::MakeBox(&m_drawingMesh);

	// Vertex Buffer
	HRenderingLibrary::CreateVertexBuffer(device, &m_drawingMesh, m_vertexBuffer);

	//Index Buffer
	HRenderingLibrary::CreateIndexBuffer(device, &m_drawingMesh, m_indexBuffer);

	//Transform Constant Buffer
	HRenderingLibrary::CreateConstantBuffer(device, &m_transformConstData, m_transformConstBuffer);

	//Shaders
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShader2.hlsl");
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShader2.hlsl");


}

void HCube3RenderingObject::Update()
{
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;

	if (MovementScalar >= MaxMovement)
	{
		IsRightDriection = false;
	}
	else if (MovementScalar <= -MaxMovement)
	{
		IsRightDriection = true;
	}

	if(IsRightDriection)
		MovementScalar += 0.01f;
	else
		MovementScalar -= 0.01f;
	
	MovementScalar = std::clamp(MovementScalar, -MaxMovement, MaxMovement);

	m_transformConstData.ModelTransform = (Matrix::CreateScale(0.05f) * Matrix::CreateRotationY(RotationYValue) * Matrix::CreateRotationX(RotationXValue) * Matrix::CreateTranslation(0.5f, MovementScalar, 0)).Transpose();

	using namespace DirectX;
	m_transformConstData.ViewTransform = XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();

	const float fovAngle = ViewAngleInDeg * (XM_PI / 180.f);
	Application* appContext = m_ParentRenderModule->GetAppContext();
	m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(fovAngle, (float)appContext->GetScreenWidth() / appContext->GetScreenHeight(), 0.01f, 100.0f);
	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();

	D3D11_MAPPED_SUBRESOURCE ms;
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->Map(m_transformConstBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &m_transformConstData, sizeof(m_transformConstData));
	context->Unmap(m_transformConstBuffer.Get(), NULL);
}

void HCube3RenderingObject::Render()
{
	// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->IASetInputLayout(m_vertexInputLayout.Get());
	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 어떤 쉐이더를 사용할지 설정
	context->VSSetShader(m_vertexShader.Get(), 0, 0);
	context->VSSetConstantBuffers(0, 1, m_transformConstBuffer.GetAddressOf());
	context->PSSetShader(m_pixelShader.Get(), 0, 0);

	context->DrawIndexed(m_drawingMesh.indices.size(), 0, 0);
}
