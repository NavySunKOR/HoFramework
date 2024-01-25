#pragma once
#include "BaseRenderingObject.h"
#include <iostream>



class HCubeLightRenderingObject : public HBaseRenderingObject
{
public:
	HCubeLightRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	float RotationYValue = 0.f;
	float RotationXValue = 0.f;
private:
	ComPtr<ID3D11Texture2D> m_Texture1;
	ComPtr<ID3D11ShaderResourceView> m_Texture1ResourceView;

	ComPtr<ID3D11Texture2D> m_Texture2;
	ComPtr<ID3D11ShaderResourceView> m_Texture2ResourceView;

	ComPtr<ID3D11Buffer> m_PSConstBuffer;
	PSConstantBuffer m_PSConstBufferData;
};