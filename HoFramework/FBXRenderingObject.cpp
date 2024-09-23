#include "FBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "SkyBoxRenderingObject.h"

void HFBXRenderingObject::Initialize()
{
	InitializeInternal();
}

void HFBXRenderingObject::Update()
{

}

void HFBXRenderingObject::Render(HCamera InCamera)
{
	RenderInternal(InCamera);
}
