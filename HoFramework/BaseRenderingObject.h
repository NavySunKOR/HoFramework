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

	//Primitive Type으로 생성 하면 무조건 외부에서 스케일을 키워줘야함.
	void ApplyMesh(EPrimitiveType InPrimitiveType);

	//아예 별도로 정의하고 싶다면 이걸 사용하면 된다.
	void ApplyMesh(Mesh InMeshData);

	//경로로 로드 하고 싶다면 이걸 사용하면 된다.
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


	//오브젝트 별 FOV 설정(FPS게임에서 건 카메라 같은 역할)
	bool m_IsUsingCustomView = false;
	bool m_CustomIsPerspective = true;
	float m_CustomFOVInDeg = 70.f;

	Matrix TranslationMatrix;
	Matrix ScaleMatrix;
	Matrix RotationMatrix;

	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

};