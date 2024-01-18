#pragma once
#include "Define.h"
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;
class HBaseRenderModule;

class HBaseRenderingObject
{
public:
	HBaseRenderingObject(HBaseRenderModule* pRenderModule)
	{
		m_ParentRenderModule = pRenderModule;
		Initialize();
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Render();

public:
	void Translate(Vector3 pTranslate);
	void Rotate(Vector3 pTranslate);
	void Scale(Vector3 pTranslate);

protected:
	Mesh m_drawingMesh;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_transformConstBuffer;
	TransformConstantBuffer m_transformConstData;

	ComPtr<ID3D11InputLayout> m_vertexInputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	HBaseRenderModule* m_ParentRenderModule;


	float ViewAngleInDeg = 70.f;
	Matrix TranslationMatrix;
	Matrix ScaleMatrix;
	Matrix RotationMatrix;

};