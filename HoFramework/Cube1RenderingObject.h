#pragma once
#include "BaseRenderingObject.h"

class HCube1RenderingObject : public HBaseRenderingObject
{
public:

	HCube1RenderingObject(HBaseRenderModule* pRenderModule) : HBaseRenderingObject(pRenderModule)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;


private:
	float RotationYValue = 0.f;
	float RotationXValue = 0.f;

};