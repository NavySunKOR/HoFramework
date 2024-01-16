#include "CustomRenderModule.h"

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//여기서부터 작성 

	return true;
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
}

void HCustomRenderModule::Render()
{
	HBaseRenderModule::Update();
	//여기서부터 작성 
}
