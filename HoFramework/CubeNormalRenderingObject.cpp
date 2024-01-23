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
	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;
	D3D11_INPUT_ELEMENT_DESC texCoord;
	D3D11_INPUT_ELEMENT_DESC normal;

	position.SemanticName = "POSITION";
	position.SemanticIndex = 0;
	position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	position.InputSlot = 0;
	position.AlignedByteOffset = 0;
	position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	position.InstanceDataStepRate = 0;

	color.SemanticName = "COLOR";
	color.SemanticIndex = 0;
	color.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	color.InputSlot = 0;
	color.AlignedByteOffset = 4 * 3;
	color.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	color.InstanceDataStepRate = 0;

	texCoord.SemanticName = "TEXCOORD";
	texCoord.SemanticIndex = 0;
	texCoord.Format = DXGI_FORMAT_R32G32_FLOAT;
	texCoord.InputSlot = 0;
	texCoord.AlignedByteOffset = 4 * 3 + 4 * 3;
	texCoord.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	texCoord.InstanceDataStepRate = 0;

	normal.SemanticName = "NORMAL";
	normal.SemanticIndex = 0;
	normal.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	normal.InputSlot = 0;
	normal.AlignedByteOffset = 4 * 3 + 4 * 3 + 4 * 2;
	normal.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	normal.InstanceDataStepRate = 0;

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position ,color,texCoord , normal };
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShaderNormal.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShaderNormal.hlsl");


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
