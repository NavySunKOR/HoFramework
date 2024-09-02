#include "Application.h"
#include "BaseRenderingObject.h"

void HBaseRenderingObject::SetVertexShader(const LPCWSTR InShaderLoc, const LPCSTR InShaderMainName)
{
	HRenderingLibrary::CreateVertexShader(m_ParentRenderModule->GetDevice(), m_vertexShader, m_vertexInputLayout, InShaderLoc, InShaderMainName,HRenderingLibrary::GetVSInputLayout());
}

void HBaseRenderingObject::SetPixelShader(const LPCWSTR InShaderLoc, const LPCSTR InShaderMainName)
{
	HRenderingLibrary::CreatePixelShader(m_ParentRenderModule->GetDevice(), m_pixelShader, InShaderLoc, InShaderMainName);
}

void HBaseRenderingObject::ApplyMesh(EPrimitiveType InPrimitiveType)
{
	Mesh mesh;


	switch (InPrimitiveType)
	{
		case EPrimitiveType::Box:
			HRenderingLibrary::MakeBox(&mesh, 1);
			break;
		case EPrimitiveType::Sphere:
			HRenderingLibrary::MakeSphere(&mesh,1.f,Vector3(0,0,0),16,16);
			break;

		case EPrimitiveType::Square:
			HRenderingLibrary::MakeSquare(&mesh);
			break;

		case EPrimitiveType::Cylinder:
			HRenderingLibrary::MakeCylinder(&mesh, 1.f, 16, 2.f);
			break;
		default: 
			assert(0);
			break;
	}


	MeshObject obj;
	obj.mesh = mesh;

	m_meshObjects.push_back(obj);

}

void HBaseRenderingObject::ApplyMesh(Mesh InMeshData)
{
	MeshObject obj;
	obj.mesh = InMeshData;

	m_meshObjects.push_back(obj);
}

void HBaseRenderingObject::ApplyMesh(const LPCSTR InDirName, const LPCSTR InFileName)
{
	auto meshes = HRenderingLibrary::LoadMeshFromFile(InDirName, InFileName);
	m_meshObjects.reserve(meshes.size());
	for (Mesh mesh : meshes)
	{
		MeshObject mObj;
		mObj.mesh = mesh;
		m_meshObjects.push_back(mObj);
	}
}

void HBaseRenderingObject::InitializeInternal()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();

	//Constant Buffers
	HRenderingLibrary::CreateConstantBuffer<BasicVSConstantBuffer>(device, m_basicVertexConstBufferData, m_basicVertexConstBuffer);

	//Constant Buffers
	HRenderingLibrary::CreateConstantBuffer<MaterialPSConstantBuffer>(device, m_materialConstBufferData, m_materialConstBuffer);


	HRenderingLibrary::CreateConstantBuffer<ViewPSConstantBuffer>(device, m_viewConstBufferData, m_viewConstBuffer);


	for (int i = 0 ; i < m_meshObjects.size() ; ++i)
	{
		// Vertex Buffer
		HRenderingLibrary::CreateVertexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].vertexBuffer);

		//Index Buffer
		HRenderingLibrary::CreateIndexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].indexBuffer);

		m_meshObjects[i].vertexConstantBuffer = m_basicVertexConstBuffer;
		m_meshObjects[i].materialPSConstantBuffer = m_materialConstBuffer;

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


	if (m_IsUsingCustomView == false)
	{
		m_basicVertexConstBufferData.ViewTransform = m_ParentRenderModule->GetMainView().GetViewMatrix();
		m_basicVertexConstBufferData.ProjectionTransform = m_ParentRenderModule->GetMainView().GetProjectionMatrix();
		m_viewConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_basicVertexConstBufferData.ViewTransform.Transpose().Invert());
	}

	HRenderingLibrary::UpdateConstantBuffer(m_basicVertexConstBufferData, m_basicVertexConstBuffer, m_ParentRenderModule->GetContext());
	HRenderingLibrary::UpdateConstantBuffer(m_materialConstBufferData, m_materialConstBuffer, m_ParentRenderModule->GetContext());
	HRenderingLibrary::UpdateConstantBuffer(m_viewConstBufferData, m_viewConstBuffer, m_ParentRenderModule->GetContext());

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

		context->PSSetConstantBuffers(0, 1, meshObj.materialPSConstantBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, m_viewConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(2, 1, m_ParentRenderModule->m_LightPSConstantBuffer.GetAddressOf());
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
