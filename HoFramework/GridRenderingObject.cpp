#include "GridRenderingObject.h"
#include "RenderingLibrary.h"
#include "BaseRenderModule.h"
#include <algorithm>
using namespace std;
void HGridRenderingObject::Initialize()
{
	HRenderingLibrary::MakeGrid(&m_drawingMesh, 25, 20, 1.5f);
	HBaseRenderingObject::Initialize();
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

	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_vertexInputLayout, L"VertexShaderGridWave.hlsl", inputs);
	HRenderingLibrary::CreatePixelShader(device, m_pixelShader, L"PixelShaderGridDraw.hlsl");

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	HRenderingLibrary::CreateConstantBuffer<PSConstantBufferGrid>(device, m_PSConstBufferData, m_PSConstBuffer);
	context->PSSetConstantBuffers(0, 1, m_PSConstBuffer.GetAddressOf());

	HRenderingLibrary::CreateConstantBuffer<VSConstantTimeBuffer>(device, m_VSTimeBufferData, m_VSTimeBuffer);
	context->VSSetConstantBuffers(1, 1, m_VSTimeBuffer.GetAddressOf());

	Scale(Vector3(0.1f, 0.1f, 0.1f));
	Translate(Vector3(-1.5f, 0.5f, 1.f));
}

void HGridRenderingObject::Update()
{
	HBaseRenderingObject::Update();

	m_VSTimeBufferData.elapsedTime += 0.1f;
	m_VSTimeBufferData.elapsedTime = (m_VSTimeBufferData.elapsedTime > 6.28f) ? 0 : m_VSTimeBufferData.elapsedTime;
	HRenderingLibrary::UpdateConstantBuffer(m_VSTimeBufferData , m_VSTimeBuffer, m_ParentRenderModule->GetContext());

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_transformConstData.ViewTransform.Transpose().Invert());
	HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, m_PSConstBuffer, m_ParentRenderModule->GetContext());
}

void HGridRenderingObject::Render()
{
	HBaseRenderingObject::Render();
	
}
