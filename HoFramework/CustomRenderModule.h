#pragma once


#include "BaseRenderModule.h"


class HCustomRenderModule : public HBaseRenderModule
{

public:
	HCustomRenderModule() : HBaseRenderModule()
	{

	}

	virtual bool Initialize(Application* pAppContext) override;

private:
	void InitSampler();

	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CreateConstantBuffer();


	void CreateVertexShader();
	void CreatePixelShader();

protected:
	virtual void Update() override;
	virtual void Render() override;


protected:
	ComPtr<ID3D11SamplerState> m_SamplerState;

protected:

	//���⼭����
	Mesh m_drawingMesh;
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_transformConstBuffer;
	TransformConstantBuffer m_transformConstData;

	ComPtr<ID3D11InputLayout> m_vertexInputLayout;
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;


	float RotationYValue;
	float RotationXValue;

	//���⼭ ���� �ϳ��� ������Ʈ



#pragma region View
protected:
	float ViewAngleInDeg = 70.f;

#pragma endregion


};