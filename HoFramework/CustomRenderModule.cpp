#include "CustomRenderModule.h"

bool HCustomRenderModule::Initialize(Application* pAppContext)
{
	if (!HBaseRenderModule::Initialize(pAppContext))
	{
		return false;
	}

	//���⼭���� �ۼ� 

	return true;
}

void HCustomRenderModule::Update()
{
	HBaseRenderModule::Update();
	//���⼭���� �ۼ� 
}

void HCustomRenderModule::Render()
{
	HBaseRenderModule::Update();
	//���⼭���� �ۼ� 
}
