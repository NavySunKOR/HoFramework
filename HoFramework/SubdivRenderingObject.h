#pragma once
#include "BaseRenderingObject.h"



class HSubdivRenderingObject : public HBaseRenderingObject
{

public:
	HSubdivRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
private:


	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	PSConstantBuffer m_PSConstBufferData;
};