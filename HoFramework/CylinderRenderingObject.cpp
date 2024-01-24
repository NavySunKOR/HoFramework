#include "CylinderRenderingObject.h"


void HCylinderRenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::MakeBox(&m_drawingMesh);

	HBaseRenderingObject::Initialize();
}

void HCylinderRenderingObject::Update()
{
	HBaseRenderingObject::Update();
}

void HCylinderRenderingObject::Render()
{
	HBaseRenderingObject::Render();
}
