#pragma once
#include "BaseRenderingObject.h"


class HCubeNormalRenderingObject : public HBaseRenderingObject
{
public:
	HCubeNormalRenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}


	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	float RotationYValue = 0.f;
	float RotationXValue = 0.f;

};