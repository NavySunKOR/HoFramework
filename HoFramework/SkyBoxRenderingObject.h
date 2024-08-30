#pragma once
#include "BaseRenderingObject.h"


class HSkyBoxRenderingObject : public HBaseRenderingObject
{
public:
	HSkyBoxRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{
	}

	virtual void Initialize();
	virtual void Update();
	virtual void Render();

	ComPtr<ID3D11ShaderResourceView> GetSkyboxDiffuse() {
		return m_SkyboxDiffuseResourceView;
	};

	ComPtr<ID3D11ShaderResourceView> GetSkyboxSpecular() {
		return m_SkyboxSpecularResourceView;
	};

	void SetSkyboxResources(LPCWSTR InDiffuseDDSLoc, LPCWSTR InSpecularDDSLoc);


private:
	ComPtr<ID3D11Resource> m_SkyboxDiffuseResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxDiffuseResourceView;
	ComPtr<ID3D11Resource> m_SkyboxSpecularResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxSpecularResourceView;
};