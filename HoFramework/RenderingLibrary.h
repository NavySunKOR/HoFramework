#pragma once
#include "Define.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <wrl.h>
#include <vector>

using Microsoft::WRL::ComPtr;
using std::shared_ptr;

class HRenderingLibrary
{

public:
	static void MakeBox(Mesh* InMesh);
	static void MakeBoxNormal(Mesh* InBoxMesh , Mesh* OutNormalMesh);
	static bool CreateIndexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pIndexBuffer);
	static bool CreateVertexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pVertexBuffer);

	template<typename T_BUFFERTYPE>
	static bool CreateConstantBuffer(ComPtr<ID3D11Device> pDeviceContext, T_BUFFERTYPE& pConstantBufferData, ComPtr<ID3D11Buffer>& pConstantBuffer)
	{
		if (sizeof(pConstantBufferData) % 16 != 0)
		{
			cout << "Const buffer must be size of 16byte!! : " << sizeof(pConstantBufferData) << endl;
			return false;
		}

		D3D11_BUFFER_DESC ConstBufferDesc = {};
		ConstBufferDesc.ByteWidth = sizeof(pConstantBufferData);
		ConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		ConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		ConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		ConstBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA ConstBufferData = {};
		ConstBufferData.pSysMem = &pConstantBufferData;

		pDeviceContext->CreateBuffer(&ConstBufferDesc, &ConstBufferData, pConstantBuffer.GetAddressOf());

		if (pConstantBuffer.Get() == nullptr)
		{
			cout << "No Const Buffer!" << endl;
			return false;
		}

		return true;
	}//TODO: ConstantBuffer�� ���ø����� �ٲ㼭 �����.
	static bool CreateVertexShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11VertexShader>& pVertexShader, ComPtr<ID3D11InputLayout>& pVertexInputLayout, LPCWSTR pShaderFileLocation, vector<D3D11_INPUT_ELEMENT_DESC> pInput);
	static bool CreatePixelShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11PixelShader>& pPixelShader, LPCWSTR pShaderFileLocation);

	template<typename T_BUFFERTYPE>
	static void UpdateConstantBuffer(T_BUFFERTYPE& pConstantBufferData, ComPtr<ID3D11Buffer>& pBuffer, ComPtr<ID3D11DeviceContext>& pContext)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		pContext->Map(pBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &pConstantBufferData, sizeof(pConstantBufferData));
		pContext->Unmap(pBuffer.Get(), NULL);
	}

	static bool CreateTexture(ComPtr<ID3D11Device> pDeviceContext, string pTextureFileLocation, ComPtr<ID3D11Texture2D>& OutTexture, ComPtr<ID3D11ShaderResourceView>& OutResourceView);
};