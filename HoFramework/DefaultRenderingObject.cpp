#include "DefaultRenderingObject.h"
#include "RenderingLibrary.h"
#include "Application.h"
#include "BaseRenderModule.h"

void HDefaultRenderingObject::Initialize()
{
	PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


	InitializeInternal();
}

void HDefaultRenderingObject::Update()
{
	UpdateInternal();
}

void HDefaultRenderingObject::Render()
{
	//TODO: �ܽ�źƮ ���� ������ ��� ã�ƺ���
	/**/

	if (UsingShaderResources.size() > 0)
	{
		ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
		ID3D11ShaderResourceView* Array[2] = { UsingShaderResources[0].Get(),UsingShaderResources[1].Get()};
		context->PSSetShaderResources(0, UsingShaderResources.size(), Array);
	}

	RenderInternal();
}

void HDefaultRenderingObject::SetExternalResource(vector<string> textureLocations)
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	UsingTextures.reserve(textureLocations.size());
	UsingShaderResources.reserve(textureLocations.size());
	for (int i = 0; i < textureLocations.size(); ++i)
	{
		ComPtr<ID3D11Texture2D> Texture;
		ComPtr<ID3D11ShaderResourceView> SRV;
		HRenderingLibrary::CreateTexture(device,textureLocations[i], Texture, SRV);
		UsingTextures.push_back(Texture);
		UsingShaderResources.push_back(SRV.Get());
	}

}