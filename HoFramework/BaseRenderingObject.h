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
	virtual void Render(HCamera InCamera) = 0;

public:
	void Translate(Vector3 pTranslate);
	void Rotate(Vector3 pTranslate);
	void Scale(Vector3 pTranslate);

	inline vector<MeshObject>& GetDrawingMeshes() { return m_meshObjects; };
	inline BasicVSConstantBuffer* GetVSConstBuffer(){return &m_basicVertexConstBufferData;}


	//Primitive Type으로 생성 하면 무조건 외부에서 스케일을 키워줘야함.
	void ApplyMesh(EPrimitiveType InPrimitiveType);

	//아예 별도로 정의하고 싶다면 이걸 사용하면 된다.
	void ApplyMesh(Mesh InMeshData);

	//경로로 로드 하고 싶다면 이걸 사용하면 된다.
	void ApplyMesh(const LPCSTR InDirName,  const LPCSTR InFileName);


	//보통 메쉬가 하나 뿐일 때 사용한다.
	void SetExternalResource(int InApplyMeshIndex, EModelTexture2DType InTextureType, string textureLocation);
	void SetSkyboxSRVs(ComPtr<ID3D11ShaderResourceView> InDiffuse, ComPtr<ID3D11ShaderResourceView> InSpecular, ComPtr<ID3D11ShaderResourceView> InBRDF);


protected:
	void InitializeInternal();
	void UpdateInternal();
	void RenderInternal(HCamera InCamera);
	void UpdateBuffer(HCamera InCamera);

	vector<MeshObject> m_meshObjects;

	ComPtr<ID3D11Buffer> m_basicVertexConstBuffer;
	ComPtr<ID3D11Buffer> m_viewConstBuffer;
	ComPtr<ID3D11Buffer> m_materialConstBuffer; //메쉬 별로 머터리얼 데이터는 다 다른데, 한번에 한번씩 그릴때 버퍼는 똑같은 버퍼를 사용하기 위하여 버퍼는 한번 만 만든다.

	vector<ComPtr<ID3D11Buffer>> m_extraVertexConstBuffers;
	vector<ComPtr<ID3D11Buffer>> m_extraPixelConstBuffers;

	BasicVSConstantBuffer m_basicVertexConstBufferData;
	ViewPSConstantBuffer m_viewConstBufferData;

	HBaseRenderModule* m_ParentRenderModule;


	ComPtr<ID3D11ShaderResourceView> IBLSRVs[3]; 
	//vector< ComPtr<ID3D11ShaderResourceView>> UsingShaderResources;


	//오브젝트 별 FOV 설정(FPS게임에서 건 카메라 같은 역할)
	bool m_IsUsingCustomView = false;
	HCamera m_CustomViewport;

	Matrix TranslationMatrix;
	Matrix ScaleMatrix;
	Matrix RotationMatrix;

};