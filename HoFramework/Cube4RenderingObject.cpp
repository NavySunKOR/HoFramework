#include "Cube4RenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include "Application.h"
#include <algorithm>


void HCube4RenderingObject::Initialize()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	HRenderingLibrary::MakeBox(&m_drawingMesh);

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

	vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position ,color,texCoord,normal};
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShaderTextureUsing.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShaderTextureUsing.hlsl");

	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/wall.jpg"), m_Texture1, m_Texture1ResourceView) == false)
	{
		cout << "wall doesn't exist! " << endl;
	}
	if (HRenderingLibrary::CreateTexture(device, string("./SampleTexture/crate.png"), m_Texture2, m_Texture2ResourceView) == false)
	{
		cout << "crate doesn't exist! " << endl;
	}


	//그 외에 정의
	Scale(Vector3(0.025f, 0.025f, 0.025f));
	Translate(Vector3(0.1f,0.1f, 0));
}

void HCube4RenderingObject::Update()
{
	//Rotation
	RotationYValue += 0.01f;
	RotationXValue += 0.01f;
	Rotate(Vector3(RotationXValue, RotationYValue, 0));

	HBaseRenderingObject::Update();
}

void HCube4RenderingObject::Render()
{
	// 버텍스/인덱스 버퍼 설정

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	ID3D11ShaderResourceView* pixelResources[2] = { m_Texture1ResourceView.Get(),
											   m_Texture2ResourceView.Get() };
	context->PSSetShaderResources(0, 2, pixelResources);
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());
	HBaseRenderingObject::Render();
}
