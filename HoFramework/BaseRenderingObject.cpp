#include "BaseRenderingObject.h"
#include "Application.h"
#include "BaseRenderModule.h"
#include "RenderingLibrary.h"

void HBaseRenderingObject::InitializeInternal()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	//Constant Buffers
	HRenderingLibrary::CreateConstantBuffer<BasicVSConstantBuffer>(device, m_basicVertexConstBufferData, m_basicVertexConstBuffer);

	//Constant Buffers
	HRenderingLibrary::CreateConstantBuffer<BasicPSConstantBuffer>(device, m_basicPixelConstBufferData, m_basicPixelConstBuffer);


	for (int i = 0 ; i < m_meshObjects.size() ; ++i)
	{
		// Vertex Buffer
		HRenderingLibrary::CreateVertexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].vertexBuffer);

		//Index Buffer
		HRenderingLibrary::CreateIndexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].indexBuffer);

		m_meshObjects[i].vertexConstantBuffer = m_basicVertexConstBuffer;
		m_meshObjects[i].pixelConstantBuffer = m_basicPixelConstBuffer;

 		if (m_meshObjects[i].mesh.textureSourceName.length() > 0)
			HRenderingLibrary::CreateTexture(device, m_meshObjects[i].mesh.textureSourceName, m_meshObjects[i].texture, m_meshObjects[i].textureResourceView);

	}

}
void HBaseRenderingObject::UpdateInternal()
{
	using namespace DirectX;

	m_basicVertexConstBufferData.ModelTransform = (ScaleMatrix * RotationMatrix * TranslationMatrix);
	m_basicVertexConstBufferData.ModelTransform = m_basicVertexConstBufferData.ModelTransform.Transpose();
	m_basicVertexConstBufferData.InverseTransform = m_basicVertexConstBufferData.ModelTransform;
	m_basicVertexConstBufferData.InverseTransform.Translation(Vector3(0.0f));
	m_basicVertexConstBufferData.InverseTransform = m_basicVertexConstBufferData.InverseTransform.Transpose().Invert();


	m_basicVertexConstBufferData.ViewTransform = Matrix::CreateRotationY(m_ParentRenderModule->GetGlobalCameraRotation().y) * Matrix::CreateTranslation(m_ParentRenderModule->GetGlobalCameraPosition());
	m_basicVertexConstBufferData.ViewTransform = m_basicVertexConstBufferData.ViewTransform.Transpose();

	HRenderingLibrary::UpdateConstantBuffer(m_basicVertexConstBufferData, m_basicVertexConstBuffer, m_ParentRenderModule->GetContext());

	bool isPerspective = (m_IsUsingCustomView) ? m_CustomIsPerspective : m_ParentRenderModule->IsPerspective();
	Application* appContext = m_ParentRenderModule->GetAppContext();

	if (isPerspective)
	{
		const float usingFOVAngle = (m_IsUsingCustomView) ? m_CustomFOVInDeg : m_ParentRenderModule->GetFOVInDeg() * (XM_PI / 180.f);
		m_basicVertexConstBufferData.ProjectionTransform = XMMatrixPerspectiveFovLH(usingFOVAngle, appContext->GetAspectRatio(), 0.01f, 100.0f);
	}
	else
	{
		m_basicVertexConstBufferData.ProjectionTransform = XMMatrixOrthographicLH(appContext->GetAspectRatio(), 0.f, 0.01f, 100.0f);
	}

	m_basicVertexConstBufferData.ProjectionTransform = m_basicVertexConstBufferData.ProjectionTransform.Transpose();

	m_basicPixelConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_basicVertexConstBufferData.ViewTransform.Transpose().Invert());

	HRenderingLibrary::UpdateConstantBuffer(m_basicVertexConstBufferData, m_basicVertexConstBuffer, m_ParentRenderModule->GetContext());
	HRenderingLibrary::UpdateConstantBuffer(m_basicPixelConstBufferData , m_basicPixelConstBuffer, m_ParentRenderModule->GetContext());

}
void HBaseRenderingObject::RenderInternal()
{
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	for (MeshObject meshObj : m_meshObjects)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetInputLayout(m_vertexInputLayout.Get());
		context->IASetVertexBuffers(0, 1, meshObj.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(meshObj.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(PrimitiveTopology);

		// 어떤 쉐이더를 사용할지 설정
		context->VSSetShader(m_vertexShader.Get(), 0, 0);
		context->VSSetConstantBuffers(0, 1, meshObj.vertexConstantBuffer.GetAddressOf());
		context->PSSetShader(m_pixelShader.Get(), 0, 0);

		if (meshObj.textureResourceView.Get())
		{
			ID3D11ShaderResourceView* pixelResources[1] = { meshObj.textureResourceView.Get() };
			context->PSSetShaderResources(0, 1, pixelResources);
		}

		context->PSSetConstantBuffers(0, 1, meshObj.pixelConstantBuffer.GetAddressOf());
		context->DrawIndexed((UINT)meshObj.mesh.indices.size(), 0, 0);
	}

}

void HBaseRenderingObject::Translate(Vector3 pTranslate)
{
	TranslationMatrix = Matrix::CreateTranslation(pTranslate);
}

void HBaseRenderingObject::Rotate(Vector3 pTranslate)
{
	RotationMatrix = Matrix::CreateRotationX(pTranslate.x) * Matrix::CreateRotationY(pTranslate.y) * Matrix::CreateRotationZ(pTranslate.z);
}

void HBaseRenderingObject::Scale(Vector3 pTranslate)
{
	ScaleMatrix = Matrix::CreateScale(pTranslate);
}
