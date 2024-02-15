#pragma once
#include "Define.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>
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
	static vector<Mesh> LoadMeshFromFile(string InDir);
	static void MakeBox(Mesh* InMesh);
	static void MakeBoxNormal(Mesh* InBoxMesh , Mesh* OutNormalMesh);
	static void MakeGrid(Mesh* InBoxMesh,int InHorizontalGridCnt, int InVerticalGridCnt,int InGridSize);
	//InRadius - 원통의 반지름 , InRadialSliceCount 원통을 세로로 쪼개짐 갯수 , InCylinderVerticalStackCount 원통의 길이
	static void MakeCylinder(Mesh* InBoxMesh, float InRadius, int InRadialSliceCount, float InCylinderHeight);
	static void MakeSphere(Mesh* InBoxMesh, float InRadius, Vector3 InCenterPos, int InVerticalDivision, int InHorizontalDivision);
	static bool CreateIndexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pIndexBuffer);
	static bool CreateVertexBuffer(ComPtr<ID3D11Device> pDeviceContext, Mesh* pDrawingMesh, ComPtr<ID3D11Buffer>& pVertexBuffer);
	static void MakeLcosahedron(Mesh* InMesh);
	static void MakeSphereSubdivision(Mesh* InMesh, float InSphereRadius);
	static void VertexNormalToFaceNormal(Vertex& v0, Vertex& v1, Vertex& v2);
	static vector<D3D11_INPUT_ELEMENT_DESC> GetVSInputLayout();

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
	}//TODO: ConstantBuffer를 템플릿으로 바꿔서 만든다.
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

	static Matrix GLMatrixToDXMatrix(aiMatrix4x4 InMatrix);

private:
	static void ProjectVertexToSphereSurface(Vertex& InVertex,const float InRadius);
	static void ProcessAINode(vector<Mesh>& OutMesh, aiNode* InNode, const aiScene* InScene,Matrix InMatrix);

	static Mesh ProcessAIMesh(aiMesh* InAIMesh, const aiScene* InScene);
};