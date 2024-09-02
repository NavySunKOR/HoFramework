#pragma once


#include "BaseRenderModule.h"
#include "ImGUIRenderSubModule.h"

class HCustomRenderModule : public HBaseRenderModule
{

public:
	HCustomRenderModule() : HBaseRenderModule()
	{

	}


	virtual bool Initialize(Application* pAppContext) override;


protected:

	virtual void InitImageFilters() override;

	void InitBlurFilter();

	virtual void Update() override;
	virtual void Render() override;



private:

	void UpdateInput();
	HImGUIRenderSubModule GUIRenderSubModule;
	bool m_MoveFPP = false;
};