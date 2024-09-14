#pragma once
#include "BaseRenderingObject.h"


class HSkyBoxRenderingObject : public HBaseRenderingObject
{
public:
	HSkyBoxRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{
	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HCamera InCamera) override;

	ComPtr<ID3D11ShaderResourceView> GetSkyboxDiffuse() {
		return m_SkyboxDiffuseResourceView;
	};

	ComPtr<ID3D11ShaderResourceView> GetSkyboxSpecular() {
		return m_SkyboxSpecularResourceView;
	};

	ComPtr<ID3D11ShaderResourceView> GetSkyboxBRDF() {
		return m_SkyboxBRDFResourceView;
	};

	void SetSkyboxResources(LPCWSTR InOriginalDDSLoc, LPCWSTR InDiffuseDDSLoc, LPCWSTR InSpecularDDSLoc, LPCWSTR InBRDFDDSLoc);


private:
	ComPtr<ID3D11Resource> m_SkyboxOriginalResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxOriginalResourceView;
	ComPtr<ID3D11Resource> m_SkyboxDiffuseResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxDiffuseResourceView;
	ComPtr<ID3D11Resource> m_SkyboxSpecularResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxSpecularResourceView;

	ComPtr<ID3D11Resource> m_SkyboxBRDFResource;
	ComPtr<ID3D11ShaderResourceView> m_SkyboxBRDFResourceView;


};