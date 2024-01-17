#include "CustomRenderModule.h"
#include "Application.h"


void HCustomRenderModule::RenderCube1()
{

	// ���ؽ�/�ε��� ���� ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_context->IASetInputLayout(m_vertexInputLayout.Get());
	m_context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// � ���̴��� ������� ����
	m_context->VSSetShader(m_vertexShader.Get(), 0, 0);
	m_context->VSSetConstantBuffers(0, 1, m_transformConstBuffer.GetAddressOf());
	m_context->PSSetShader(m_pixelShader.Get(), 0, 0);

	m_context->DrawIndexed(m_drawingMesh.indices.size(), 0, 0);
}
void HCustomRenderModule::RenderCube2()
{

}