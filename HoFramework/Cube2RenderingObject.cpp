#include "Cube2RenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include "Application.h"

void HCube2RenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	HRenderingLibrary::MakeBox(&m_drawingMesh);

	// Vertex Buffer
	HRenderingLibrary::CreateVertexBuffer(device, &m_drawingMesh, m_vertexBuffer);

	//Index Buffer
	HRenderingLibrary::CreateIndexBuffer(device, &m_drawingMesh, m_indexBuffer);

	//Transform Constant Buffer
	HRenderingLibrary::CreateConstantBuffer(device, &m_transformConstData, m_transformConstBuffer);

	//Shaders
	D3D11_INPUT_ELEMENT_DESC position;
	D3D11_INPUT_ELEMENT_DESC color;
	D3D11_INPUT_ELEMENT_DESC texCoord;

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

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position ,color,texCoord };
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShader.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShader.hlsl");


}

void HCube2RenderingObject::Update()
{
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;

	Rotate(Vector3(RotationXValue, RotationYValue, 0));
	Translate(Vector3(0.2f, 0.2f, 0));
	Scale(Vector3(0.1f, 0.1f, 0.1f));

	HBaseRenderingObject::Update();

}

void HCube2RenderingObject::Render()
{
	// 버텍스/인덱스 버퍼 설정
	HBaseRenderingObject::Render();
}
