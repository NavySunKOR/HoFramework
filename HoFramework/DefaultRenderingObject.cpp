#include "DefaultRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "BaseRenderModule.h"

void HDefaultRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	InitializeInternal();
}

void HDefaultRenderingObject::Update()
{
	UpdateInternal();
}

void HDefaultRenderingObject::Render(HCamera InCamera)
{
	//TODO: 콘스탄트 버퍼 설정할 방법 찾아볼것
	/**/

	RenderInternal(InCamera);
}