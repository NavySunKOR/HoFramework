#pragma once
#include "BaseRenderingObject.h"


struct VSConstantTimeBuffer
{
	float elapsedTime = 0.f;
	Vector3 Dummy;
};

struct PSConstantBufferGrid
{

	//32
	Light UsingLight;

	//12
	Vector3 UsingViewPosition;
	float Dummy;

	//
	Material UsingMat;
};

class HGridRenderingObject : public HBaseRenderingObject
{
public:
	HGridRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	PSConstantBufferGrid m_PSConstBufferData;

	ComPtr<ID3D11Buffer> m_VSTimeBuffer;
	VSConstantTimeBuffer m_VSTimeBufferData;

	




};