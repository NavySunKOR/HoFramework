#include "CustomRenderModule.h"
#include "Application.h"

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//여기서부터 작성 


    InitSampler();

    // Vertex Buffer
	D3D11_BUFFER_DESC VertexBufferDesc = {};
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA VertexBufferData = {};
	VertexBufferData.pSysMem = m_drawingMesh.vertices.data();

	m_device->CreateBuffer(&VertexBufferDesc, &VertexBufferData, m_vertexBuffer.GetAddressOf());

	//Index Buffer
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint32_t) * m_drawingMesh.indices.size();
	IndexBufferDesc.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA IndexBufferData = {};
	IndexBufferData.pSysMem = m_drawingMesh.indices.data();

	m_device->CreateBuffer(&IndexBufferDesc, &IndexBufferData, m_indexBuffer.GetAddressOf());

	//Transform Constant Buffer

	D3D11_BUFFER_DESC ConstBufferDesc = {};
	VertexBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ConstBufferData = {};
	ConstBufferData.pSysMem = &m_transformConstData;

	m_device->CreateBuffer(&ConstBufferDesc, &ConstBufferData, m_transformConstBuffer.GetAddressOf());




	return true;
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
	RoationYValue += 0.01f;

	m_transformConstData.ModelTransform = (Matrix::CreateScale(1.f) * Matrix::CreateRotationY(RoationYValue)).Transpose();

	using namespace DirectX;
	m_transformConstData.ViewTransform = XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();

	const float fovAngle = ViewAngleInDeg * (XM_PI / 180.f);
	m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(fovAngle,  (float)m_AppContext->GetScreenWidth() / m_AppContext->GetScreenHeight(), 0.01f, 100.0f);
	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();

	D3D11_MAPPED_SUBRESOURCE ms;
	m_context->Map(m_transformConstBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &m_transformConstData, sizeof(m_transformConstData));
	m_context->Unmap(m_transformConstBuffer.Get(), NULL);

}

void HCustomRenderModule::Render()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
}
