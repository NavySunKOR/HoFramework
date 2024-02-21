#pragma once
#include "BaseRenderingObject.h"


struct ExtraPixelBuffer
{
	float RimIntensity = 0.f;
	float RimPow = 0.f;
	float Dummy1 = 0.f;
	float Dummy2 = 0.f;
};

class HFBXDragonRenderingObject : public HBaseRenderingObject
{

public:
	HFBXDragonRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	ExtraPixelBuffer m_PSExtraData;

private:
	vector<MeshObject> m_Meshes;
	PSConstantBuffer m_PSConstBufferData;


	ComPtr<ID3D11Buffer> m_PSExtraBuffer;


};