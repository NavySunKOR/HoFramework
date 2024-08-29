#pragma once
#include "Define.h"
#include <d3d11.h>
#include <wrl.h>
#include "BaseRenderModule.h"
#include "RenderingLibrary.h"

using Microsoft::WRL::ComPtr;
class HBaseRenderModule;


class HBaseRenderingObject
{
public:
	HBaseRenderingObject(HBaseRenderModule* pRenderModule)
	{
		m_ParentRenderModule = pRenderModule;
	}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

public:
	void Translate(Vector3 pTranslate);
	void Rotate(Vector3 pTranslate);
	void Scale(Vector3 pTranslate);

	inline vector<MeshObject>* GetDrawingMeshes() { return &m_meshObjects; };
	inline BasicVSConstantBuffer* GetVSConstBuffer(){return &m_basicVertexConstBufferData;}
	inline BasicPSConstantBuffer* GetPSConstBuffer() { return &m_basicPixelConstBufferData; }

	void SetVertexShader(const LPCWSTR InShaderLoc, const LPCSTR InShaderMainName);
	void SetPixelShader(const LPCWSTR InShaderLoc, const LPCSTR InShaderMainName);

	//Primitive Type���� ���� �ϸ� ������ �ܺο��� �������� Ű�������.
	void ApplyMesh(EPrimitiveType InPrimitiveType);

	//�ƿ� ������ �����ϰ� �ʹٸ� �̰� ����ϸ� �ȴ�.
	void ApplyMesh(Mesh InMeshData);

	//��η� �ε� �ϰ� �ʹٸ� �̰� ����ϸ� �ȴ�.
	void ApplyMesh(const LPCSTR InDirName,  const LPCSTR InFileName);

protected:

	void InitializeInternal();
	void UpdateInternal();
	void RenderInternal();

	vector<MeshObject> m_meshObjects;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11InputLayout> m_vertexInputLayout;
	ComPtr<ID3D11PixelShader> m_pixelShader;


	ComPtr<ID3D11Buffer> m_basicVertexConstBuffer;
	ComPtr<ID3D11Buffer> m_basicPixelConstBuffer;

	vector<ComPtr<ID3D11Buffer>> m_extraVertexConstBuffers;
	vector<ComPtr<ID3D11Buffer>> m_extraPixelConstBuffers;

	BasicVSConstantBuffer m_basicVertexConstBufferData;
	BasicPSConstantBuffer m_basicPixelConstBufferData;

	HBaseRenderModule* m_ParentRenderModule;


	//������Ʈ �� FOV ����(FPS���ӿ��� �� ī�޶� ���� ����)
	bool m_IsUsingCustomView = false;
	bool m_CustomIsPerspective = true;
	float m_CustomFOVInDeg = 70.f;

	Matrix TranslationMatrix;
	Matrix ScaleMatrix;
	Matrix RotationMatrix;

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

};