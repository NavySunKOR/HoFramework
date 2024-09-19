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


	//Primitive Type���� ���� �ϸ� ������ �ܺο��� �������� Ű�������.
	void ApplyMesh(EPrimitiveType InPrimitiveType);

	//�ƿ� ������ �����ϰ� �ʹٸ� �̰� ����ϸ� �ȴ�.
	void ApplyMesh(Mesh InMeshData);

	//��η� �ε� �ϰ� �ʹٸ� �̰� ����ϸ� �ȴ�.
	void ApplyMesh(const LPCSTR InDirName,  const LPCSTR InFileName);


	//���� �޽��� �ϳ� ���� �� ����Ѵ�.
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
	ComPtr<ID3D11Buffer> m_materialConstBuffer; //�޽� ���� ���͸��� �����ʹ� �� �ٸ���, �ѹ��� �ѹ��� �׸��� ���۴� �Ȱ��� ���۸� ����ϱ� ���Ͽ� ���۴� �ѹ� �� �����.

	vector<ComPtr<ID3D11Buffer>> m_extraVertexConstBuffers;
	vector<ComPtr<ID3D11Buffer>> m_extraPixelConstBuffers;

	BasicVSConstantBuffer m_basicVertexConstBufferData;
	ViewPSConstantBuffer m_viewConstBufferData;

	HBaseRenderModule* m_ParentRenderModule;


	ComPtr<ID3D11ShaderResourceView> IBLSRVs[3]; 
	//vector< ComPtr<ID3D11ShaderResourceView>> UsingShaderResources;


	//������Ʈ �� FOV ����(FPS���ӿ��� �� ī�޶� ���� ����)
	bool m_IsUsingCustomView = false;
	HCamera m_CustomViewport;

	Matrix TranslationMatrix;
	Matrix ScaleMatrix;
	Matrix RotationMatrix;

};