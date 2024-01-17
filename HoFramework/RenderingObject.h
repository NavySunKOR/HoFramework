#pragma once
#include "Define.h"
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class HRenderingObject
{
public:
	HRenderingObject(ComPtr<ID3D11Device>& pDxdevice)
	{
		m_device = pDxdevice;
		Initialize();
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Render();



protected:
	Mesh m_drawingMesh;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_transformConstBuffer;
	TransformConstantBuffer m_transformConstData;

	ComPtr<ID3D11InputLayout> m_vertexInputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;

	ComPtr<ID3D11Device> m_device;


};