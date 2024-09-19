#pragma once
#include "RenderingLibrary.h"
#include "Define.h"
#include "BaseRenderModule.h"
using namespace DirectX::SimpleMath;

class HImageFilter
{
public:
	virtual void Initialize(HBaseRenderModule* ParentModule, int width, int height);
	virtual void Update();
	virtual void Render();


    void UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context) {
        HRenderingLibrary::UpdateConstantBuffer<SamplingPixelConstantData>(m_pixelConstData, m_mesh->materialPSConstantBuffer, context);
    }

    void SetShaderResources(
        const std::vector<ComPtr<ID3D11ShaderResourceView>>& resources) {

        m_shaderResources.clear();
        for (const auto& res : resources) {
            m_shaderResources.push_back(res.Get());
        }
    }

    void SetRenderTargets(
        const std::vector<ComPtr<ID3D11RenderTargetView>>& targets) {

        m_renderTargets.clear();
        for (const auto& tar : targets) {
            m_renderTargets.push_back(tar.Get());
        }
    }


    ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    struct SamplingPixelConstantData {
        float dx;
        float dy;
        float threshold;
        float strength;

        float gamma;
        float exposure;
        float dummy1;
        float dummy2;
    };

    SamplingPixelConstantData m_pixelConstData;


protected:
	std::shared_ptr<MeshObject> m_mesh;

    HBaseRenderModule* m_parentRenderModule = nullptr;

    D3D11_VIEWPORT m_viewport;

    // Do not delete pointers
    std::vector<ID3D11ShaderResourceView*> m_shaderResources;
    std::vector<ID3D11RenderTargetView*> m_renderTargets;
	
};