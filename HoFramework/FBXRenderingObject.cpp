#include "FBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "SkyBoxRenderingObject.h"

void HFBXRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	InitializeInternal();
}

void HFBXRenderingObject::Update()
{
	UpdateInternal();
}

void HFBXRenderingObject::Render(HCamera InCamera)
{
	RenderInternal(InCamera);
}
