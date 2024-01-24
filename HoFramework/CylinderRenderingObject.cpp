#include "CylinderRenderingObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void HCylinderRenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::MakeCylinder(&m_drawingMesh,3.f,16,10.f);

	HBaseRenderingObject::Initialize();

	//Shaders
	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/BaseDiffuse/VertexShaderUsingLight.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/Cylinder/PixelShaderCylinder.hlsl");

	Scale(Vector3(0.1f,0.1f, 0.1f));

	Rotate(Vector3(90.f * (XM_PI / 180.f), 0.f, 0.f));
}

void HCylinderRenderingObject::Update()
{
	HBaseRenderingObject::Update();
}

void HCylinderRenderingObject::Render()
{
	HBaseRenderingObject::Render();
}
