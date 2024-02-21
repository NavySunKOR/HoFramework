#include "FBXDragonRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"

void HFBXDragonRenderingObject::Initialize()
{
	auto meshes = HRenderingLibrary::LoadMeshFromFile("./Meshes/zeldaPosed001/","zeldaPosed001.fbx");
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ComPtr<ID3D11Device>& Device = m_ParentRenderModule->GetDevice();

	m_PSConstBufferData.UsingLight.LightDir = Vector3(0.f, 0.f, 1.f);
	m_PSConstBufferData.UsingLight.LightIntensity = 1.f;
	m_PSConstBufferData.UsingLight.LightPos = Vector3(0.f, 0.f, -2.f);

	m_PSConstBufferData.UsingMat.ambient = Vector3(0.3f, 0.3f, 0.3f);
	m_PSConstBufferData.UsingMat.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	m_PSConstBufferData.UsingMat.shiness = 1.f;
	m_PSConstBufferData.UsingMat.specular = Vector3(1.f);

	HRenderingLibrary::CreateVertexShader(Device, m_vertexShader, m_vertexInputLayout, L"./Shaders/Dragon/VertexShaderDragon.hlsl", HRenderingLibrary::GetVSInputLayout());
	HRenderingLibrary::CreatePixelShader(Device, m_pixelShader, L"./Shaders/Dragon/PixelShaderDragon.hlsl");

	Scale(Vector3(0.01f, 0.01f, 0.01f));
	Translate(Vector3(0.f, -1.f, 0.f));

	m_PSExtraData.RimIntensity = 5.f;
	m_PSExtraData.RimPow = 5.f;

	HRenderingLibrary::CreateConstantBuffer(Device, m_PSExtraData, m_PSExtraBuffer);

	for (Mesh mesh : meshes)
	{
		MeshObject mObj;
		mObj.mesh = mesh;		
		
		HRenderingLibrary::CreateVertexBuffer(Device, &mObj.mesh, mObj.vertexBuffer);
		HRenderingLibrary::CreateIndexBuffer(Device, &mObj.mesh, mObj.indexBuffer);
		HRenderingLibrary::CreateConstantBuffer(Device, m_transformConstData, mObj.vertexConstantBuffer);
		HRenderingLibrary::CreateConstantBuffer(Device, m_PSConstBufferData, mObj.pixelConstantBuffer);

		if(mObj.mesh.textureSourceName.empty() == false)
			HRenderingLibrary::CreateTexture(Device, mObj.mesh.textureSourceName, mObj.texture, mObj.textureResourceView);

		m_Meshes.push_back(mObj);
	}

}

void HFBXDragonRenderingObject::Update()
{
	using namespace DirectX;
	m_transformConstData.ModelTransform = (ScaleMatrix * RotationMatrix * TranslationMatrix);
	m_transformConstData.ModelTransform = m_transformConstData.ModelTransform.Transpose();
	m_transformConstData.InverseTransform = m_transformConstData.ModelTransform;
	m_transformConstData.InverseTransform.Translation(Vector3(0.0f));
	m_transformConstData.InverseTransform = m_transformConstData.InverseTransform.Transpose().Invert();


	m_transformConstData.ViewTransform = Matrix::CreateRotationY(m_ParentRenderModule->GetGlobalCameraRotation().y) * Matrix::CreateTranslation(m_ParentRenderModule->GetGlobalCameraPosition());
	m_transformConstData.ViewTransform = m_transformConstData.ViewTransform.Transpose();

	bool isPerspective = (m_IsUsingCustomView) ? m_CustomIsPerspective : m_ParentRenderModule->IsPerspective();
	Application* appContext = m_ParentRenderModule->GetAppContext();

	if (isPerspective)
	{
		const float usingFOVAngle = (m_IsUsingCustomView) ? m_CustomFOVInDeg : m_ParentRenderModule->GetFOVInDeg() * (XM_PI / 180.f);
		m_transformConstData.ProjectionTransform = XMMatrixPerspectiveFovLH(usingFOVAngle, appContext->GetAspectRatio(), 0.01f, 100.0f);
	}
	else
	{
		m_transformConstData.ProjectionTransform = XMMatrixOrthographicLH(appContext->GetAspectRatio(), 0.f, 0.01f, 100.0f);
	}

	m_transformConstData.ProjectionTransform = m_transformConstData.ProjectionTransform.Transpose();

	m_PSConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_transformConstData.ViewTransform.Transpose().Invert());

	for (MeshObject mesh : m_Meshes)
	{
		HRenderingLibrary::UpdateConstantBuffer(m_transformConstData, mesh.vertexConstantBuffer, m_ParentRenderModule->GetContext());
		HRenderingLibrary::UpdateConstantBuffer(m_PSConstBufferData, mesh.pixelConstantBuffer, m_ParentRenderModule->GetContext());
	}


	HRenderingLibrary::UpdateConstantBuffer(m_PSExtraData, m_PSExtraBuffer, m_ParentRenderModule->GetContext());
}

void HFBXDragonRenderingObject::Render()
{
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	context->IASetInputLayout(m_vertexInputLayout.Get());

	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());

	for (MeshObject mesh : m_Meshes)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->IASetPrimitiveTopology(PrimitiveTopology);

		// 어떤 쉐이더를 사용할지 설정
		context->VSSetShader(m_vertexShader.Get(), 0, 0);
		context->VSSetConstantBuffers(0, 1, mesh.vertexConstantBuffer.GetAddressOf());
		context->PSSetConstantBuffers(0, 1, mesh.pixelConstantBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, m_PSExtraBuffer.GetAddressOf());
		context->PSSetShader(m_pixelShader.Get(), 0, 0);

		if (mesh.textureResourceView.Get())
		{
			ID3D11ShaderResourceView* pixelResources[1] = { mesh.textureResourceView.Get() };
			context->PSSetShaderResources(0, 1, pixelResources);
		}

		context->DrawIndexed(mesh.mesh.indices.size(), 0, 0);
	}
}
