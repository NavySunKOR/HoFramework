#pragma once
#include "ImageFilter.h"


class HSeaImageFilter : public HImageFilter
{
public:
	virtual void Initialize(HBaseRenderModule* ParentModule, const wstring vertexShaderName, const wstring pixelShaderName,int width, int height);
	virtual void Update() override;
	virtual void Render() override;


	struct SeaPixelConstantBuffer
	{
	public:
		float iTime;
		float Dummy1;
		float Dummy2;
		float Dummy3;
	};

	SeaPixelConstantBuffer Sea;

private:
	ComPtr<ID3D11Buffer> m_SeaPixelConstantBuffer;

};