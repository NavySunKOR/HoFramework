#include "CustomRenderModule.h"
#include "Application.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")


void MakeBox(Mesh* InMesh)
{
	InMesh->vertices.reserve(4);

	for (int i = 0; i < 4; ++i)
	{
		Vertex NewVert;
		NewVert.color = Vector3(1, 0, 0);
		InMesh->vertices.push_back(NewVert);
		
	}

	//왼쪽 아래부터 시계방향


	//앞면
	InMesh->vertices[0].position = Vector3(-1.f, -1.f, 0.f);
	InMesh->vertices[1].position = Vector3(-1.f, 1.f, 0.f);
	InMesh->vertices[2].position = Vector3(1.f, 1.f, 0.f);
	InMesh->vertices[3].position = Vector3(-1.f, 1.f, -1.f);

	InMesh->indices.push_back(0);
	InMesh->indices.push_back(1);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(0);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(3);


	////뒷면
	//InMesh->vertices[4].position = Vector3(-1.f, -1.f, -1.f);
	//InMesh->vertices[5].position = Vector3(-1.f, 1.f, -1.f);
	//InMesh->vertices[6].position = Vector3(1.f, 1.f, -1.f);
	//InMesh->vertices[7].position = Vector3(-1.f, 1.f, -1.f);

	//InMesh->indices.push_back(4);
	//InMesh->indices.push_back(5);
	//InMesh->indices.push_back(6);
	//InMesh->indices.push_back(4);
	//InMesh->indices.push_back(6);
	//InMesh->indices.push_back(7);


	////왼쪽
	//InMesh->vertices[8].position = Vector3(-1.f, -1.f, -1.f);
	//InMesh->vertices[9].position = Vector3(-1.f, 1.f, -1.f);
	//InMesh->vertices[10].position = Vector3(-1.f, 1.f, 1.f);
	//InMesh->vertices[11].position = Vector3(-1.f, -1.f, 1.f);

	//InMesh->indices.push_back(8);
	//InMesh->indices.push_back(9);
	//InMesh->indices.push_back(10);
	//InMesh->indices.push_back(8);
	//InMesh->indices.push_back(10);
	//InMesh->indices.push_back(11);

	////오른쪽
	//InMesh->vertices[12].position = Vector3(1.f, -1.f, -1.f);
	//InMesh->vertices[13].position = Vector3(1.f, 1.f, -1.f);
	//InMesh->vertices[14].position = Vector3(1.f, 1.f, 1.f);
	//InMesh->vertices[15].position = Vector3(1.f, -1.f, 1.f);

	//InMesh->indices.push_back(12);
	//InMesh->indices.push_back(13);
	//InMesh->indices.push_back(14);
	//InMesh->indices.push_back(12);
	//InMesh->indices.push_back(14);
	//InMesh->indices.push_back(15);


	////위
	//InMesh->vertices[16].position = Vector3(-1.f, 1.f, 1.f);
	//InMesh->vertices[17].position = Vector3(-1.f, 1.f, -1.f);
	//InMesh->vertices[18].position = Vector3(1.f, 1.f, -1.f);
	//InMesh->vertices[19].position = Vector3(1.f, 1.f, 1.f);

	//InMesh->indices.push_back(16);
	//InMesh->indices.push_back(17);
	//InMesh->indices.push_back(18);
	//InMesh->indices.push_back(16);
	//InMesh->indices.push_back(18);
	//InMesh->indices.push_back(19);

	////아래
	//InMesh->vertices[20].position = Vector3(-1.f, -1.f, 1.f);
	//InMesh->vertices[21].position = Vector3(-1.f, -1.f, -1.f);
	//InMesh->vertices[22].position = Vector3(1.f, -1.f, -1.f);
	//InMesh->vertices[23].position = Vector3(1.f, -1.f, 1.f);

	//InMesh->indices.push_back(20);
	//InMesh->indices.push_back(21);
	//InMesh->indices.push_back(22);
	//InMesh->indices.push_back(20);
	//InMesh->indices.push_back(22);
	//InMesh->indices.push_back(23);

}

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//여기서부터 작성 


    InitSampler();

	MakeBox(&m_drawingMesh);

    // Vertex Buffer
	D3D11_BUFFER_DESC VertexBufferDesc = {};
	VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertexBufferDesc.ByteWidth = sizeof(Vertex) * m_drawingMesh.vertices.size();
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA VertexBufferData = {};
	VertexBufferData.pSysMem = m_drawingMesh.vertices.data();

	m_device->CreateBuffer(&VertexBufferDesc, &VertexBufferData, m_vertexBuffer.GetAddressOf());

	if (m_vertexBuffer.Get() == nullptr)
	{
		cout << "No Vertex Buffer!" << endl;
	}

	//Index Buffer
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint32_t) * m_drawingMesh.indices.size();
	IndexBufferDesc.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA IndexBufferData = {};
	IndexBufferData.pSysMem = m_drawingMesh.indices.data();

	m_device->CreateBuffer(&IndexBufferDesc, &IndexBufferData, m_indexBuffer.GetAddressOf());


	if (m_indexBuffer.Get() == nullptr)
	{
		cout << "No Index Buffer!" << endl;
	}

	//Transform Constant Buffer

	D3D11_BUFFER_DESC ConstBufferDesc = {};
	ConstBufferDesc.ByteWidth = sizeof(m_transformConstData);
	ConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ConstBufferData = {};
	ConstBufferData.pSysMem = &m_transformConstData;

	m_device->CreateBuffer(&ConstBufferDesc, &ConstBufferData, m_transformConstBuffer.GetAddressOf());

	if (m_transformConstBuffer.Get() == nullptr)
	{
		cout << "No Const Buffer!" << endl;
	}

	//Shaders
	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;

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

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position,color };
	
	{
		ComPtr<ID3DBlob> VSBlob;
		ComPtr<ID3DBlob> VSErrorBlob;

		HRESULT VShr = D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &VSBlob, &VSErrorBlob);

		if (FAILED(VShr))
		{
			cout << (char*)VSErrorBlob->GetBufferPointer() << endl;
			return false;
		}

		m_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
		m_device->CreateInputLayout(inputs.data(), UINT(inputs.size()), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), m_vertexInputLayout.GetAddressOf());
	}
	

	{
		ComPtr<ID3DBlob> PSBlob;
		ComPtr<ID3DBlob> PSErrorBlob;

		HRESULT PShr = D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &PSBlob, &PSErrorBlob);

		if (FAILED(PShr))
		{
			cout << (char*)PSErrorBlob->GetBufferPointer() << endl;
			return false;
		}

		m_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
	}

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
	HBaseRenderModule::Render();
	//여기서부터 작성 
	// IA: Input-Assembler stage
	// VS: Vertex Shader
	// PS: Pixel Shader
	// RS: Rasterizer stage
	// OM: Output-Merger stage.



	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_context->IASetInputLayout(m_vertexInputLayout.Get());
	m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 어떤 쉐이더를 사용할지 설정
	m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
	m_context->VSSetConstantBuffers(0, 1, m_transformConstBuffer.GetAddressOf());
	m_context->PSSetShader(m_pixelShader.Get(), 0, 0);

	m_context->RSSetState(m_RasterizerState.Get());
	m_context->RSSetViewports(1, &m_screenViewport);

	// 비교: Depth Buffer를 사용하지 않는 경우
	// m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);

	m_context->DrawIndexed(m_drawingMesh.indices.size(), 0, 0);


	m_swapChain->Present(1, 0);
}
