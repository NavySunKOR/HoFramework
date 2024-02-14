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
	ComPtr<ID3D11Texture2D> m_Texture;
	ComPtr<ID3D11ShaderResourceView> m_TextureResourceView;
	

	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	PSConstantBuffer m_PSConstBufferData;

	float RotX = 0.f;
};