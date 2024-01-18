#include "Cube1RenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include "Application.h"

void HCube1RenderingObject::Initialize()
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
	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;
	D3D11_INPUT_ELEMENT_DESC texCoord;

	position.SemanticName = "POSITION";
	position.SemanticIndex = 0;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = 0;
	position.AlignedByteOffset = 0;
	position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	position.InstanceDataStepRate = 0;

	color.SemanticName = "COLOR";
	color.SemanticIndex = 0;
	color.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	color.InputSlot = 0;
	color.AlignedByteOffset = 4 * 3;
	color.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	color.InstanceDataStepRate = 0;

	texCoord.SemanticName = "TEXCOORD";
	texCoord.SemanticIndex = 0;
	texCoord.Format = DXGI_FORMAT_R32G32_FLOAT;
	texCoord.InputSlot = 0;
	texCoord.AlignedByteOffset = 4 * 3 + 4 * 3;
	texCoord.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	texCoord.InstanceDataStepRate = 0;

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position ,color,texCoord };

	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShader.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShader.hlsl");
}

void HCube1RenderingObject::Update()
{
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;

	m_transformConstData.ModelTransform = (Matrix::CreateScale(0.1f) * Matrix::CreateRotationY(RotationYValue) * Matrix::CreateRotationX(RotationXValue)).Transpose();

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

void HCube1RenderingObject::Render()
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
