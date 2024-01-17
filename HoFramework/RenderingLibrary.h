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
	static bool CreateIndexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pIndexBuffer);
	static bool CreateVertexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pVertexBuffer);
	static bool CreateConstantBuffer(ComPtr<ID3D11Device> pDeviceContext, TransformConstantBuffer* pConstantBufferData, ComPtr<ID3D11Buffer>& pConstantBuffer); //TODO: ConstantBuffer를 템플릿으로 바꿔서 만든다.
	static bool CreateVertexShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11VertexShader>& pVertexShader, ComPtr<ID3D11InputLayout>& pVertexInputLayout, LPCWSTR pShaderFileLocation);
	static bool CreatePixelShader(ComPtr<ID3D11Device> pDeviceContext, ComPtr<ID3D11PixelShader>& pPixelShader, LPCWSTR pShaderFileLocation);
};