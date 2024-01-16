#pragma once


#include "BaseRenderModule.h"

class HCustomRenderModule : HBaseRenderModule
{

public:
	virtual bool Initialize(Application* pAppContext) override;

protected:
	virtual void Update() override;
	virtual void Render() override;


};