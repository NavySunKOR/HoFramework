#pragma once
#include "BaseRenderingObject.h"


struct ExtraPixelBuffer
{
	float RimIntensity = 0.f;
	float RimPow = 0.f;
	float Dummy1 = 0.f;
	float Dummy2 = 0.f;
};

class HFBXRenderingObject : public HBaseRenderingObject
{

public:
	HFBXRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	ExtraPixelBuffer m_PSExtraData;

private:
	vector<MeshObject> m_Meshes;


};