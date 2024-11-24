#pragma once
#include "PSO.h"

class Application;

namespace Graphics
{

    namespace States {
        extern ComPtr<ID3D11SamplerState> linearWrapSS;
        extern ComPtr<ID3D11SamplerState> linearClampSS;

        extern ComPtr<ID3D11RasterizerState> solidRS;
        extern ComPtr<ID3D11RasterizerState> wireRS;
        extern ComPtr<ID3D11RasterizerState> solidCCWRS;
        extern ComPtr<ID3D11RasterizerState> wireCCWRS;

        extern ComPtr<ID3D11BlendState> translucentBS;

        extern ComPtr<ID3D11VertexShader> basicVS;
        extern ComPtr<ID3D11VertexShader> skyboxVS;
        extern ComPtr<ID3D11VertexShader> postProcessBaseVS;
        extern ComPtr<ID3D11VertexShader> depthOnlyVS;

        extern ComPtr<ID3D11PixelShader> basicPS;
        extern ComPtr<ID3D11PixelShader> skyboxPS;
        extern ComPtr<ID3D11PixelShader> postProcessBasePS;
        extern ComPtr<ID3D11PixelShader> depthOnlyPS;
        extern ComPtr<ID3D11PixelShader> blurXPPPS;
        extern ComPtr<ID3D11PixelShader> blurYPPPS;

        extern ComPtr<ID3D11InputLayout> basicIL;
        extern ComPtr<ID3D11InputLayout> skyboxIL;
        extern ComPtr<ID3D11InputLayout> postProcessBaseIL;

        extern ComPtr<ID3D11DepthStencilState> depthStencilState;

        extern HGraphicsPSO basicSolidPSO;
        extern HGraphicsPSO basicWirePSO;

        extern HGraphicsPSO postProcessBasePSO;
        extern HGraphicsPSO skyboxPSO;

        void InitStates(ComPtr<ID3D11Device>& device);

        //SamplerStates
        void InitSamplerStates(ComPtr<ID3D11Device>& device);


        //SamplerStates
        void InitRasterizerState(ComPtr<ID3D11Device>& device);

        void InitBlendStates(ComPtr<ID3D11Device>& device);

        void InitVertexShaders(ComPtr<ID3D11Device>& device);

        void InitPixelShaders(ComPtr<ID3D11Device>& device);

    };

    namespace Defines
    {
        extern D3D11_VIEWPORT screenViewport;
        extern D3D11_VIEWPORT shadowViewport;

        void InitDefines(Application* InAppContext);
    }
}