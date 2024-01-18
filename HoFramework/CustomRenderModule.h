#pragma once


#include "BaseRenderModule.h"


class HCustomRenderModule : public HBaseRenderModule
{

public:
	HCustomRenderModule() : HBaseRenderModule()
	{

	}


	virtual bool Initialize(Application* pAppContext) override;

protected:
	virtual void Update() override;
	virtual void Render() override;

};