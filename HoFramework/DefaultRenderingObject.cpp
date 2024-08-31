#include "DefaultRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HDefaultRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	InitializeInternal();
}

void HDefaultRenderingObject::Update()
{
	UpdateInternal();
}

void HDefaultRenderingObject::Render()
{
	//TODO: �ܽ�źƮ ���� ������ ��� ã�ƺ���
	/**/
	RenderInternal();
}
