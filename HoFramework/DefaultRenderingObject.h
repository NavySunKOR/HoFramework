#pragma once
#include "BaseRenderingObject.h"


class HDefaultRenderingObject : public HBaseRenderingObject
{

public:
	HDefaultRenderingObject(HBaseRenderModule* InModuleRenderer) : HBaseRenderingObject(InModuleRenderer)
	{

	}

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render(HCamera InCamera) override;

};