#pragma once
#include "BaseRenderingObject.h"


class HDefaultRenderingObject : public HBaseRenderingObject
{

public:
	HDefaultRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	void SetExternalResource(vector<string> textureLocations);
	void AddShaderResources(vector<ComPtr<ID3D11ShaderResourceView>> texturesToAdd);
private:

	vector<ComPtr<ID3D11Texture2D>> UsingTextures;
	vector< ComPtr<ID3D11ShaderResourceView>> UsingShaderResources;


};