
#include "RenderingLibrary.h"

#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void HRenderingLibrary::MakeBox(Mesh* InMesh)
{
	InMesh->vertices.reserve(24);

	for (int i = 0; i < 24; ++i)
	{
		InMesh->vertices.push_back(Vertex());
	}

	//왼쪽 아래부터 시계방향

	//윗면
	InMesh->vertices[0].position = Vector3(-1.0f, 1.0f, -1.0f);
	InMesh->vertices[1].position = Vector3(-1.0f, 1.0f, 1.0f);
	InMesh->vertices[2].position = Vector3(1.0f, 1.0f, 1.0f);
	InMesh->vertices[3].position = Vector3(1.0f, 1.0f, -1.0f);
	InMesh->vertices[0].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[1].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[2].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[3].color = Vector3(0.0f, 0.0f, 0.0f);
	InMesh->vertices[0].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[1].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[2].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[3].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[0].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[1].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[2].normal = Vector3(0.0f, 1.f, 0.0f);
	InMesh->vertices[3].normal = Vector3(0.0f, 1.f, 0.0f);


	InMesh->indices.push_back(0);
	InMesh->indices.push_back(1);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(0);
	InMesh->indices.push_back(2);
	InMesh->indices.push_back(3);


	//아랫면
	InMesh->vertices[4].position = Vector3(-1.0f, -1.0f, -1.0f);
	InMesh->vertices[5].position = Vector3(1.0f, -1.0f, -1.0f);
	InMesh->vertices[6].position = Vector3(1.0f, -1.0f, 1.0f);
	InMesh->vertices[7].position = Vector3(-1.0f, -1.0f, 1.0f);
	InMesh->vertices[4].color = Vector3(0, 0, 0);
	InMesh->vertices[5].color = Vector3(0, 0, 0);
	InMesh->vertices[6].color = Vector3(0, 0, 0);
	InMesh->vertices[7].color = Vector3(0, 0, 0);
	InMesh->vertices[4].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[5].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[6].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[7].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[4].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[5].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[6].normal = Vector3(0.0f, -1.0f, 0.0f);
	InMesh->vertices[7].normal = Vector3(0.0f, -1.0f, 0.0f);

	InMesh->indices.push_back(4);
	InMesh->indices.push_back(5);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(4);
	InMesh->indices.push_back(6);
	InMesh->indices.push_back(7);


	// 앞면
	InMesh->vertices[8].position = Vector3(-1.0f, -1.0f, -1.0f);
	InMesh->vertices[9].position = Vector3(-1.0f, 1.0f, -1.0f);
	InMesh->vertices[10].position = Vector3(1.0f, 1.0f, -1.0f);
	InMesh->vertices[11].position = Vector3(1.0f, -1.0f, -1.0f);
	InMesh->vertices[8].color = Vector3(0, 0, 0);
	InMesh->vertices[9].color = Vector3(0, 0, 0);
	InMesh->vertices[10].color = Vector3(0, 0, 0);
	InMesh->vertices[11].color = Vector3(0, 0, 0);
	InMesh->vertices[8].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[9].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[10].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[11].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[8].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[9].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[10].normal = Vector3(0.0f, 0.0f, -1.0f);
	InMesh->vertices[11].normal = Vector3(0.0f, 0.0f, -1.0f);

	InMesh->indices.push_back(8);
	InMesh->indices.push_back(9);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(8);
	InMesh->indices.push_back(10);
	InMesh->indices.push_back(11);

	// 뒷면
	InMesh->vertices[12].position = Vector3(-1.0f, -1.0f, 1.0f);
	InMesh->vertices[13].position = Vector3(1.0f, -1.0f, 1.0f);
	InMesh->vertices[14].position = Vector3(1.0f, 1.0f, 1.0f);
	InMesh->vertices[15].position = Vector3(-1.0f, 1.0f, 1.0f);
	InMesh->vertices[12].color = Vector3(0, 0, 0);
	InMesh->vertices[13].color = Vector3(0, 0, 0);
	InMesh->vertices[14].color = Vector3(0, 0, 0);
	InMesh->vertices[15].color = Vector3(0, 0, 0);
	InMesh->vertices[12].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[13].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[14].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[15].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[12].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[13].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[14].normal = Vector3(0.0f, 0.0f, 1.0f);
	InMesh->vertices[15].normal = Vector3(0.0f, 0.0f, 1.0f);

	InMesh->indices.push_back(12);
	InMesh->indices.push_back(13);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(12);
	InMesh->indices.push_back(14);
	InMesh->indices.push_back(15);


	////왼쪽
	InMesh->vertices[16].position = Vector3(-1.0f, -1.0f, 1.0f);
	InMesh->vertices[17].position = Vector3(-1.0f, 1.0f, 1.0f);
	InMesh->vertices[18].position = Vector3(-1.0f, 1.0f, -1.0f);
	InMesh->vertices[19].position = Vector3(-1.0f, -1.0f, -1.0f);
	InMesh->vertices[16].color = Vector3(0, 0, 0);
	InMesh->vertices[17].color = Vector3(0, 0, 0);
	InMesh->vertices[18].color = Vector3(0, 0, 0);
	InMesh->vertices[19].color = Vector3(0, 0, 0);
	InMesh->vertices[16].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[17].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[18].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[19].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[16].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[17].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[18].normal = Vector3(-1.0f, 0.0f, 0.0f);
	InMesh->vertices[19].normal = Vector3(-1.0f, 0.0f, 0.0f);

	InMesh->indices.push_back(16);
	InMesh->indices.push_back(17);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(16);
	InMesh->indices.push_back(18);
	InMesh->indices.push_back(19);

	////오른쪽	
	InMesh->vertices[20].position = Vector3(1.0f, -1.0f, 1.0f);
	InMesh->vertices[21].position = Vector3(1.0f, -1.0f, -1.0f);
	InMesh->vertices[22].position = Vector3(1.0f, 1.0f, -1.0f);
	InMesh->vertices[23].position = Vector3(1.0f, 1.0f, 1.0f);
	InMesh->vertices[20].color = Vector3(0, 0, 0);
	InMesh->vertices[21].color = Vector3(0, 0, 0);
	InMesh->vertices[22].color = Vector3(0, 0, 0);
	InMesh->vertices[23].color = Vector3(0, 0, 0);
	InMesh->vertices[20].texCoord = Vector2(0.0f, 0.0f);
	InMesh->vertices[21].texCoord = Vector2(1.0f, 0.0f);
	InMesh->vertices[22].texCoord = Vector2(1.0f, 1.0f);
	InMesh->vertices[23].texCoord = Vector2(0.0f, 1.0f);
	InMesh->vertices[20].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[21].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[22].normal = Vector3(1.0f, 0.0f, 0.0f);
	InMesh->vertices[23].normal = Vector3(1.0f, 0.0f, 0.0f);

	//이 메쉬의 중심점

	//Vector3 center = Vector3(0.f);

	//for (int i = 0 ; i < InMesh->vertices.size() ;++i)
	//{
	//	center += InMesh->vertices[i].position;
	//}

	//center /= InMesh->vertices.size();


	////메쉬 to 버텍스가 노말
	//for (int i = 0; i < InMesh->vertices.size(); ++i)
	//{
	//	InMesh->vertices[i].normal = (InMesh->vertices[i].position - center);
	//	InMesh->vertices[i].normal.Normalize();
	//}

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

bool HRenderingLibrary::CreateVertexShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11VertexShader>& pVertexShader, ComPtr<ID3D11InputLayout>& pVertexInputLayout , LPCWSTR pShaderFileLocation , vector<D3D11_INPUT_ELEMENT_DESC> pInput)
{

	ComPtr<ID3DBlob> VSBlob;
	ComPtr<ID3DBlob> VSErrorBlob;

	HRESULT VShr = D3DCompileFromFile(pShaderFileLocation, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &VSBlob, &VSErrorBlob);

	if (FAILED(VShr))
	{
		cout << (char*)VSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, pVertexShader.GetAddressOf());
	pDeviceContext->CreateInputLayout(pInput.data(), UINT(pInput.size()), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), pVertexInputLayout.GetAddressOf());

	return true;
}

bool HRenderingLibrary::CreatePixelShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11PixelShader>& pPixelShader, LPCWSTR pShaderFileLocation)
{
	ComPtr<ID3DBlob> PSBlob;
	ComPtr<ID3DBlob> PSErrorBlob;

	HRESULT PShr = D3DCompileFromFile(pShaderFileLocation, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &PSBlob, &PSErrorBlob);

	if (FAILED(PShr))
	{
		cout << (char*)PSErrorBlob->GetBufferPointer() << endl;
		return false;
	}

	pDeviceContext->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
	return true;
}

bool HRenderingLibrary::CreateTexture(ComPtr<ID3D11Device> pDeviceContext, string pTextureFileLocation, ComPtr<ID3D11Texture2D>& OutTexture, ComPtr<ID3D11ShaderResourceView>& OutResourceView)
{
	int width, height, channels;
	unsigned char* img = stbi_load(pTextureFileLocation.c_str(), &width, &height, &channels,0);

	if (img == nullptr)
	{
		cout << "Image Doesn't exist!" << endl;
		return false;
	}

	std::vector<uint8_t> colors;

	colors.resize(size_t(width * height * channels));

	memcpy(colors.data(), img, colors.size() * sizeof(uint8_t));

	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = colors.data();
	InitData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * channels;

	pDeviceContext->CreateTexture2D(&txtDesc, &InitData, OutTexture.GetAddressOf());
	pDeviceContext->CreateShaderResourceView(OutTexture.Get(), nullptr,	OutResourceView.GetAddressOf());

	return (OutTexture.Get() && OutResourceView.Get());
}