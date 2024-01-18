#include "BaseRenderingObject.h"
#include "Application.h"
#include "BaseRenderModule.h"
#include "RenderingLibrary.h"

void HBaseRenderingObject::Initialize()
{

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	// Vertex Buffer
	HRenderingLibrary::CreateVertexBuffer(device, &m_drawingMesh, m_vertexBuffer);

	//Index Buffer
	HRenderingLibrary::CreateIndexBuffer(device, &m_drawingMesh, m_indexBuffer);

	//Transform Constant Buffer
	HRenderingLibrary::CreateConstantBuffer(device, &m_transformConstData, m_transformConstBuffer);

}

void HBaseRenderingObject::Update()
{
	using namespace DirectX;

	m_transformConstData.ModelTransform = (ScaleMatrix * RotationMatrix * TranslationMatrix).Transpose();

	m_transformConstData.ViewTransform = XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();


	bool isPerspective = (m_IsUsingCustomView) ? m_CustomIsPerspective : m_ParentRenderModule->IsPerspective();
	Application* appContext = m_ParentRenderModule->GetAppContext();

	if (isPerspective)
	{
		const float usingFOVAngle = (m_IsUsingCustomView) ? m_CustomFOVInDeg : m_ParentRenderModule->GetFOVInDeg() *(XM_PI / 180.f);
		m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(usingFOVAngle, appContext->GetAspectRatio(), 0.01f, 100.0f);
	}
	else
	{
		m_transformConstData.ProjectionTransform = XMMatrixOrthographicLH(appContext->GetAspectRatio(),0.f, 0.01f, 100.0f);
	}

	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();


	D3D11_MAPPED_SUBRESOURCE ms;
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->Map(m_transformConstBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &m_transformConstData, sizeof(m_transformConstData));
	context->Unmap(m_transformConstBuffer.Get(), NULL);
}

void HBaseRenderingObject::Render()
{
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

void HBaseRenderingObject::Translate(Vector3 pTranslate)
{
	TranslationMatrix = Matrix::CreateTranslation(pTranslate);
}

void HBaseRenderingObject::Rotate(Vector3 pTranslate)
{
	RotationMatrix = Matrix::CreateRotationX(pTranslate.x) * Matrix::CreateRotationY(pTranslate.y) * Matrix::CreateRotationZ(pTranslate.z);
}

void HBaseRenderingObject::Scale(Vector3 pTranslate)
{
	ScaleMatrix = Matrix::CreateScale(pTranslate);
}
