#pragma once
#include "BaseRenderingObject.h"

class HCylinderRenderingObject :public HBaseRenderingObject
{
public:
	HCylinderRenderingObject(HBaseRenderModule* pRenderModule) :  HBaseRenderingObject(pRenderModule)
	{

	}
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;



};