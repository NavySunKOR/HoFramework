
#pragma once
#include "BaseRenderingObject.h"


class HEnvMapSphereRenderingObject : public HBaseRenderingObject
{
public:
	HEnvMapSphereRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	BasicPSConstantBuffer m_PSConstBufferData;

	ComPtr<ID3D11ShaderResourceView> m_SkyboxResourceView;
};