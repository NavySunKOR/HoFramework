#pragma once
#include "BaseRenderingObject.h"


class HCube4RenderingObject : public HBaseRenderingObject
{
public:
	HCube4RenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	ComPtr<ID3D11Texture2D> m_Texture1; 
	ComPtr<ID3D11ShaderResourceView> m_Texture1ResourceView;

	ComPtr<ID3D11Texture2D> m_Texture2;
	ComPtr<ID3D11ShaderResourceView> m_Texture2ResourceView;

	float RotationYValue = 0.f;
	float RotationXValue = 0.f;

};