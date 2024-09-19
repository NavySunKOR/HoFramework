#include "DefaultRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "BaseRenderModule.h"

void HDefaultRenderingObject::Initialize()
{
	InitializeInternal();
}

void HDefaultRenderingObject::Update()
{
	UpdateInternal();
}

void HDefaultRenderingObject::Render(HCamera InCamera)
{
	//TODO: �ܽ�źƮ ���� ������ ��� ã�ƺ���
	/**/

	RenderInternal(InCamera);
}