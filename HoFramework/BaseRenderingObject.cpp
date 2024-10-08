#include "Application.h"
#include "BaseRenderingObject.h"

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

void HBaseRenderingObject::SetExternalResource(int InApplyMeshIndex, EModelTexture2DType InTextureType, string textureLocation)
{
	m_meshObjects[InApplyMeshIndex].mesh.textureNames[(int)InTextureType] = textureLocation;

	switch (InTextureType)
	{
		case EModelTexture2DType::Albedo:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseAlbedoMap = true;
			break;
		case EModelTexture2DType::Normal:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseNormalMap = true;
			break;
		case EModelTexture2DType::Height:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseHeightMap = true;
			break;
		case EModelTexture2DType::AmbientOcclusion:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseAmbientOcclusionMap = true;
			break;
		case EModelTexture2DType::Roughness:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseRoughnessMap = true;
			break;
		case EModelTexture2DType::Metallic:
			m_meshObjects[InApplyMeshIndex].materialPSConstantData.Mat.UseMetallicMap = true;
			break;
		default:
			cout << "No texture define" << endl;
			break;
	}
}

void HBaseRenderingObject::SetSkyboxSRVs(ComPtr<ID3D11ShaderResourceView> InDiffuse, ComPtr<ID3D11ShaderResourceView> InSpecular, ComPtr<ID3D11ShaderResourceView> InBRDF)
{
	IBLSRVs[0] = InDiffuse;
	IBLSRVs[1] = InSpecular;
	IBLSRVs[2] = InBRDF;


	for (int i = 0; i < m_meshObjects.size(); ++i)
	{
		//PBR On이면 BRDF포함 , 아니면 미포함
		if ( (m_meshObjects[i].materialPSConstantData.Mat.UsePBR && IBLSRVs[0].Get() && IBLSRVs[1].Get() && IBLSRVs[2].Get())
			|| (!m_meshObjects[i].materialPSConstantData.Mat.UsePBR && IBLSRVs[0].Get() && IBLSRVs[1].Get()))
			m_meshObjects[i].materialPSConstantData.Mat.UseIBL = true;
	}
}

void HBaseRenderingObject::UpdateBuffer(HCamera InCamera)
{
	m_basicVertexConstBufferData.WorldTransform = (ScaleMatrix * RotationMatrix * TranslationMatrix);
	m_basicVertexConstBufferData.WorldTransform = m_basicVertexConstBufferData.WorldTransform.Transpose();
	m_basicVertexConstBufferData.InverseTransform = m_basicVertexConstBufferData.WorldTransform;
	m_basicVertexConstBufferData.InverseTransform.Translation(Vector3(0.0f));
	m_basicVertexConstBufferData.InverseTransform = m_basicVertexConstBufferData.InverseTransform.Transpose().Invert();


	if (m_IsUsingCustomView == false)
	{
		m_basicVertexConstBufferData.ViewTransform = InCamera.GetViewMatrix();
		m_basicVertexConstBufferData.InvViewTransform = m_basicVertexConstBufferData.ViewTransform.Invert();
		m_basicVertexConstBufferData.ProjTransform = InCamera.GetProjectionMatrix();
		m_basicVertexConstBufferData.InvProjTransform = m_basicVertexConstBufferData.ProjTransform.Invert();
		m_basicVertexConstBufferData.ViewProjTransform = (m_basicVertexConstBufferData.ViewTransform * m_basicVertexConstBufferData.ProjTransform).Transpose();
		m_basicVertexConstBufferData.InvViewProjTransform = m_basicVertexConstBufferData.ViewProjTransform.Invert();
		m_viewConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_basicVertexConstBufferData.ViewTransform.Invert());
	}
	else
	{
		m_basicVertexConstBufferData.ViewTransform = m_CustomViewport.GetViewMatrix();
		m_basicVertexConstBufferData.InvViewTransform = m_basicVertexConstBufferData.ViewTransform.Invert();
		m_basicVertexConstBufferData.ProjTransform = m_CustomViewport.GetProjectionMatrix();
		m_basicVertexConstBufferData.InvProjTransform = m_basicVertexConstBufferData.ProjTransform.Invert();
		m_basicVertexConstBufferData.ViewProjTransform = (m_basicVertexConstBufferData.ViewTransform * m_basicVertexConstBufferData.ProjTransform).Transpose();
		m_basicVertexConstBufferData.InvViewProjTransform = m_basicVertexConstBufferData.ViewProjTransform.Invert();
		m_viewConstBufferData.UsingViewPosition = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_basicVertexConstBufferData.ViewTransform.Invert());
	}

	HRenderingLibrary::UpdateConstantBuffer(m_basicVertexConstBufferData, m_basicVertexConstBuffer, m_ParentRenderModule->GetContext());
	HRenderingLibrary::UpdateConstantBuffer(m_viewConstBufferData, m_viewConstBuffer, m_ParentRenderModule->GetContext());
}


void HBaseRenderingObject::InitializeInternal()
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	//Constant Buffers
	HRenderingLibrary::CreateConstantBuffer<BasicVSConstantBuffer>(device, m_basicVertexConstBufferData, m_basicVertexConstBuffer);

	//Constant Buffers
	MaterialPSConstantBuffer emptyData;
	HRenderingLibrary::CreateConstantBuffer<MaterialPSConstantBuffer>(device, emptyData, m_materialConstBuffer);


	HRenderingLibrary::CreateConstantBuffer<ViewPSConstantBuffer>(device, m_viewConstBufferData, m_viewConstBuffer);


	for (int i = 0 ; i < m_meshObjects.size() ; ++i)
	{
		// Vertex Buffer
		HRenderingLibrary::CreateVertexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].vertexBuffer);

		//Index Buffer
		HRenderingLibrary::CreateIndexBuffer(device, &m_meshObjects[i].mesh, m_meshObjects[i].indexBuffer);

		m_meshObjects[i].vertexConstantBuffer = m_basicVertexConstBuffer;
		m_meshObjects[i].materialPSConstantBuffer = m_materialConstBuffer;

		for (int j = 0; j < 6; ++j)
		{
			if (m_meshObjects[i].mesh.textureNames[j].size() > 0)
			{
				HRenderingLibrary::CreateTexture(device, context, m_meshObjects[i].mesh.textureNames[j], m_meshObjects[i].textures[j], m_meshObjects[i].textureResourceViews[j]);
				
				switch (j)
				{
					case EModelTexture2DType::Albedo:
						m_meshObjects[i].materialPSConstantData.Mat.UseAlbedoMap = true;
						break;
					case EModelTexture2DType::Normal: 
						m_meshObjects[i].materialPSConstantData.Mat.UseNormalMap = true;
						break;
					case EModelTexture2DType::Height:
						m_meshObjects[i].materialPSConstantData.Mat.UseHeightMap = true;
						break;
					case EModelTexture2DType::AmbientOcclusion:
						m_meshObjects[i].materialPSConstantData.Mat.UseAmbientOcclusionMap = true;
						break;
					case EModelTexture2DType::Roughness:
						m_meshObjects[i].materialPSConstantData.Mat.UseRoughnessMap = true;
						break;
					case EModelTexture2DType::Metallic: 
						m_meshObjects[i].materialPSConstantData.Mat.UseMetallicMap = true;
						break;
					default:
						cout << "No texture define" << endl;
					break;
				}
				
			}
				
		}
	}

}

void HBaseRenderingObject::RenderInternal(HCamera InCamera)
{
	UpdateBuffer(InCamera);
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	for (MeshObject meshObj : m_meshObjects)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, meshObj.vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(meshObj.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		HRenderingLibrary::UpdateConstantBuffer(meshObj.materialPSConstantData, m_materialConstBuffer, m_ParentRenderModule->GetContext());

		// 어떤 쉐이더를 사용할지 설정
		context->VSSetConstantBuffers(0, 1, meshObj.vertexConstantBuffer.GetAddressOf());

		ID3D11ShaderResourceView* pixelResources[9] = 
		{ meshObj.textureResourceViews[0].Get() ,meshObj.textureResourceViews[1].Get() ,meshObj.textureResourceViews[2].Get(), 
		  meshObj.textureResourceViews[3].Get() ,meshObj.textureResourceViews[4].Get() ,meshObj.textureResourceViews[5].Get(),
		  IBLSRVs[0].Get(),IBLSRVs[1].Get(),IBLSRVs[2].Get()
		};

		context->PSSetShaderResources(0, 9, pixelResources);

		context->PSSetConstantBuffers(0, 1, meshObj.materialPSConstantBuffer.GetAddressOf());
		context->PSSetConstantBuffers(1, 1, m_viewConstBuffer.GetAddressOf());
		context->PSSetConstantBuffers(2, 1, m_ParentRenderModule->GetLightPSConstantBuffer().GetAddressOf());
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
