#pragma once
#include "BaseRenderingObject.h"


class HSphereRenderingObject : public HBaseRenderingObject
{
public:
	HSphereRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:


	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	PSConstantBuffer m_PSConstBufferData;
};