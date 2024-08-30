#include "FBXRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "SkyBoxRenderingObject.h"

void HFBXRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	InitializeInternal();
}

void HFBXRenderingObject::Update()
{
	UpdateInternal();
}

void HFBXRenderingObject::Render()
{
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	for (MeshObject meshObj : m_meshObjects)
	{

		if (meshObj.textureResourceView.Get())
		{
			ID3D11ShaderResourceView* pixelResources[1] = { meshObj.textureResourceView.Get()};
			context->PSSetShaderResources(0, 1, pixelResources);
		}

		ID3D11ShaderResourceView* pixelResources[2] = { m_ParentRenderModule->GetSkybox()->GetSkyboxDiffuse().Get()  ,m_ParentRenderModule->GetSkybox()->GetSkyboxSpecular().Get() };
		context->PSSetShaderResources(1, 2, pixelResources);
	}

	RenderInternal();
}
