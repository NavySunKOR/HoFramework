#pragma once
#include "BaseRenderingObject.h"


class HCubeMapRenderingObject : public HBaseRenderingObject
{
public:
	HCubeMapRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Render();
private:
	ComPtr<ID3D11Resource> m_SkyboxResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxResourceView;
};