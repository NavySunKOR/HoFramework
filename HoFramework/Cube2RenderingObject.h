#pragma once
#include "BaseRenderingObject.h"

class HCube2RenderingObject : public HBaseRenderingObject
{
public:
	HCube2RenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	float RotationYValue;
	float RotationXValue;
	float ViewAngleInDeg = 70.f;


};