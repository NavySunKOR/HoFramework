
#include "RenderingLibrary.h"

void HRenderingLibrary::MakeBox(Mesh* InMesh)
{
	InMesh->vertices.reserve(24);

	for (int i = 0; i < 24; ++i)
	{
		InMesh->vertices.push_back(Vertex());
	}

	//왼쪽 아래부터 시계방향

	//앞면
	InMesh->vertices[0].position = Vector3(-1.0f, 1.0f, 1.0f);
	InMesh->vertices[1].position = Vector3(-1.0f, -1.0f, 1.0f);
	InMesh->vertices[2].position = Vector3(1.0f, -1.0f, 1.0f);
	InMesh->vertices[3].position = Vector3(1.0f, 1.0f, 1.0f);
	InMesh->vertices[0].color = Vector3(0, 1.f, 0);
	InMesh->vertices[1].color = Vector3(0, 0, 0);
	InMesh->vertices[2].color = Vector3(1.f, 0, 0);
	InMesh->vertices[3].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[0].texCoord = Vector2(0, 1.f);
	InMesh->vertices[1].texCoord = Vector2(0, 0);
	InMesh->vertices[2].texCoord = Vector2(1.f, 0);
	InMesh->vertices[3].texCoord = Vector2(1.f, 1.f);


	InMesh->indices.push_back(0);
	InMesh->indices.push_back(1);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(0);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(3);


	////뒷면
	InMesh->vertices[4].position = Vector3(1.f, -1.f, -1.f);
	InMesh->vertices[5].position = Vector3(1.f, 1.f, -1.f);
	InMesh->vertices[6].position = Vector3(-1.f, 1.f, -1.f);
	InMesh->vertices[7].position = Vector3(-1.f, -1.f, -1.f);
	InMesh->vertices[4].color = Vector3(0, 1.f, 0);
	InMesh->vertices[5].color = Vector3(0, 0, 0);
	InMesh->vertices[6].color = Vector3(1.f, 0, 0);
	InMesh->vertices[7].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[4].texCoord = Vector2(0, 1.f);
	InMesh->vertices[5].texCoord = Vector2(0, 0);
	InMesh->vertices[6].texCoord = Vector2(1.f, 0);
	InMesh->vertices[7].texCoord = Vector2(1.f, 1.f);

	InMesh->indices.push_back(4);
	InMesh->indices.push_back(5);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(4);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(7);


	////왼쪽
	InMesh->vertices[8].position = Vector3(-1.f, -1.f, -1.f);
	InMesh->vertices[9].position = Vector3(-1.f, 1.f, -1.f);
	InMesh->vertices[10].position = Vector3(-1.f, 1.f, 1.f);
	InMesh->vertices[11].position = Vector3(-1.f, -1.f, 1.f);
	InMesh->vertices[8].color = Vector3(0, 1.f, 0);
	InMesh->vertices[9].color = Vector3(0, 0, 0);
	InMesh->vertices[10].color = Vector3(1.f, 0, 0);
	InMesh->vertices[11].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[8].texCoord = Vector2(0, 1.f);
	InMesh->vertices[9].texCoord = Vector2(0, 0);
	InMesh->vertices[10].texCoord = Vector2(1.f, 0);
	InMesh->vertices[11].texCoord = Vector2(1.f, 1.f);

	InMesh->indices.push_back(8);
	InMesh->indices.push_back(9);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(8);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(11);

	////오른쪽
	InMesh->vertices[12].position = Vector3(1.f, -1.f, -1.f);
	InMesh->vertices[13].position = Vector3(1.f, 1.f, -1.f);
	InMesh->vertices[14].position = Vector3(1.f, 1.f, 1.f);
	InMesh->vertices[15].position = Vector3(1.f, -1.f, 1.f);
	InMesh->vertices[12].color = Vector3(0, 1.f, 0);
	InMesh->vertices[13].color = Vector3(0, 0, 0);
	InMesh->vertices[14].color = Vector3(1.f, 0, 0);
	InMesh->vertices[15].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[12].texCoord = Vector2(0, 1.f);
	InMesh->vertices[13].texCoord = Vector2(0, 0);
	InMesh->vertices[14].texCoord = Vector2(1.f, 0);
	InMesh->vertices[15].texCoord = Vector2(1.f, 1.f);

	InMesh->indices.push_back(12);
	InMesh->indices.push_back(13);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(12);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(15);


	////위
	InMesh->vertices[16].position = Vector3(-1.f, 1.f, 1.f);
	InMesh->vertices[17].position = Vector3(-1.f, 1.f, -1.f);
	InMesh->vertices[18].position = Vector3(1.f, 1.f, -1.f);
	InMesh->vertices[19].position = Vector3(1.f, 1.f, 1.f);
	InMesh->vertices[16].color = Vector3(0, 1.f, 0);
	InMesh->vertices[17].color = Vector3(0, 0, 0);
	InMesh->vertices[18].color = Vector3(1.f, 0, 0);
	InMesh->vertices[19].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[16].texCoord = Vector2(0, 1.f);
	InMesh->vertices[17].texCoord = Vector2(0, 0);
	InMesh->vertices[18].texCoord = Vector2(1.f, 0);
	InMesh->vertices[19].texCoord = Vector2(1.f, 1.f);

	InMesh->indices.push_back(16);
	InMesh->indices.push_back(17);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(16);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(19);

	////아래	
	InMesh->vertices[20].position = Vector3(-1.f, -1.f, 1.f);
	InMesh->vertices[21].position = Vector3(-1.f, -1.f, -1.f);
	InMesh->vertices[22].position = Vector3(1.f, -1.f, -1.f);
	InMesh->vertices[23].position = Vector3(1.f, -1.f, 1.f);
	InMesh->vertices[20].color = Vector3(0, 1.f, 0);
	InMesh->vertices[21].color = Vector3(0, 0, 0);
	InMesh->vertices[22].color = Vector3(1.f, 0, 0);
	InMesh->vertices[23].color = Vector3(1.f, 1.f, 0);
	InMesh->vertices[20].texCoord = Vector2(0, 1.f);
	InMesh->vertices[21].texCoord = Vector2(0, 0);
	InMesh->vertices[22].texCoord = Vector2(1.f, 0);
	InMesh->vertices[23].texCoord = Vector2(1.f, 1.f);

	InMesh->indices.push_back(20);
	InMesh->indices.push_back(21);
	InMesh->indices.push_back(22);
	InMesh->indices.push_back(20);
	InMesh->indices.push_back(22);
	InMesh->indices.push_back(23);
}

bool HRenderingLibrary::CreateVertexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer> &pVertexBuffer)
{
    D3D11_BUFFER_DESC VertexBufferDesc = {};
    VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    VertexBufferDesc.ByteWidth = sizeof(Vertex) * pDrawingMesh->vertices.size();
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA VertexBufferData = {};
    VertexBufferData.pSysMem = pDrawingMesh->vertices.data();

    pDeviceContext->CreateBuffer(&VertexBufferDesc, &VertexBufferData, pVertexBuffer.GetAddressOf());

    if (pVertexBuffer.Get() == nullptr)
    {
        cout << "No Vertex Buffer!" << endl;
        return false;
    }
    return true;
}

bool HRenderingLibrary::CreateConstantBuffer(ComPtr<ID3D11Device> pDeviceContext, TransformConstantBuffer* pConstantBufferData, ComPtr<ID3D11Buffer> &pConstantBuffer)
{
	D3D11_BUFFER_DESC ConstBufferDesc = {};
	ConstBufferDesc.ByteWidth = sizeof(*pConstantBufferData);
	ConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	ConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ConstBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA ConstBufferData = {};
	ConstBufferData.pSysMem = pConstantBufferData;

	pDeviceContext->CreateBuffer(&ConstBufferDesc, &ConstBufferData, pConstantBuffer.GetAddressOf());

	if (pConstantBuffer.Get() == nullptr)
	{
		cout << "No Const Buffer!" << endl;
		return false;
	}

	return true;
}

bool HRenderingLibrary::CreateIndexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer> &pIndexBuffer)
{
	D3D11_BUFFER_DESC IndexBufferDesc = {};
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.ByteWidth = sizeof(uint16_t) * pDrawingMesh->indices.size();
	IndexBufferDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA IndexBufferData = {};
	IndexBufferData.pSysMem = pDrawingMesh->indices.data();

	pDeviceContext->CreateBuffer(&IndexBufferDesc, &IndexBufferData, pIndexBuffer.GetAddressOf());

	if (pIndexBuffer.Get() == nullptr)
	{
		cout << "No Index Buffer!" << endl;
		return false;
	}

	return true;
}

bool HRenderingLibrary::CreateVertexShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11VertexShader>& pVertexShader, ComPtr<ID3D11InputLayout>& pVertexInputLayout , LPCWSTR pShaderFileLocation)
{
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
	texCoord.AlignedByteOffset = 4 * 3 +  4 * 3;
	texCoord.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	texCoord.InstanceDataStepRate = 0;

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position,color,texCoord };

	ComPtr<ID3DBlob> VSBlob;
	ComPtr<ID3DBlob> VSErrorBlob;

	HRESULT VShr = D3DCompileFromFile(pShaderFileLocation, 0, 0, "main", "vs_5_0", 0, 0, &VSBlob, &VSErrorBlob);

	if (FAILED(VShr))
	{
		cout << (char*)VSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	pDeviceContext->CreateInputLayout(inputs.data(), UINT(inputs.size()), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), pVertexInputLayout.GetAddressOf());

	return true;
}

bool HRenderingLibrary::CreatePixelShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11PixelShader>& pPixelShader, LPCWSTR pShaderFileLocation)
{
	ComPtr<ID3DBlob> PSBlob;
	ComPtr<ID3DBlob> PSErrorBlob;

	HRESULT PShr = D3DCompileFromFile(pShaderFileLocation, 0, 0, "main", "ps_5_0", 0, 0, &PSBlob, &PSErrorBlob);

	if (FAILED(PShr))
	{
		cout << (char*)PSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	return true;
}
