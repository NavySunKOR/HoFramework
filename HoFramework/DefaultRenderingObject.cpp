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
	//TODO: 콘스탄트 버퍼 설정할 방법 찾아볼것
	/**/

	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();

	if (UsingShaderResources.size() > 0)
	{
		ID3D11ShaderResourceView* Array[2] = { UsingShaderResources[0].Get(),UsingShaderResources[1].Get()};
		context->PSSetShaderResources(0, UsingShaderResources.size(), Array);
	}
	context->PSSetSamplers(0, 1, m_ParentRenderModule->GetSampler().GetAddressOf());

	RenderInternal();
}

void HDefaultRenderingObject::SetExternalResource(vector<string> textureLocations)
{
	ComPtr<ID3D11Device>& device = m_ParentRenderModule->GetDevice();
	ComPtr<ID3D11DeviceContext>& context = m_ParentRenderModule->GetContext();
	UsingTextures.reserve(textureLocations.size());
	UsingShaderResources.reserve(textureLocations.size());
	for (int i = 0; i < textureLocations.size(); ++i)
	{
		ComPtr<ID3D11Texture2D> Texture;
		ComPtr<ID3D11ShaderResourceView> SRV;
		HRenderingLibrary::CreateTexture(device,context,textureLocations[i], Texture, SRV);
		UsingTextures.push_back(Texture);
		UsingShaderResources.push_back(SRV.Get());
	}

}