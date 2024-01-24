#include "CubeNormalRenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"

void HCubeNormalRenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	Mesh CubeMesh;
	HRenderingLibrary::MakeBox(&CubeMesh);
	HRenderingLibrary::MakeBoxNormal(&CubeMesh, &m_drawingMesh);

	HBaseRenderingObject::Initialize();

	//Shaders
	vector<D3D11_INPUT_ELEMENT_DESC> inputs = HRenderingLibrary::GetVSInputLayout();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"./Shaders/Normal/VertexShaderNormal.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"./Shaders/Normal/PixelShaderNormal.hlsl");


	Scale(Vector3(0.5f, 0.5f, 0.5f));
	Translate(Vector3(0.f, -0.3f, 1.f));


	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

}

void HCubeNormalRenderingObject::Update()
{
	Rotate(Vector3(RotationXValue, RotationYValue, 0.f));
	HBaseRenderingObject::Update();
}

void HCubeNormalRenderingObject::Render()
{
	HBaseRenderingObject::Render();
}
