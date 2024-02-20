#pragma once
#include "BaseRenderingObject.h"


class HLoadFBXRenderingObject : public HBaseRenderingObject
{
public:
	HLoadFBXRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
private:

	vector<MeshObject> m_Meshes;


	PSConstantBuffer m_PSConstBufferData;

};