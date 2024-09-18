#pragma once
#include "Define.h"

class GraphicsPSO
{
public:
	GraphicsPSO();

    void SetBlendState(ComPtr<ID3D11BlendState> InState) { m_blendState = InState; }
    void SetRasterizerState(ComPtr<ID3D11RasterizerState> InState) { m_rasterizerState = InState; }
    void SetDepthStencilState(ComPtr<ID3D11DepthStencilState> InDepthStencilState) { m_depthStencilState = InDepthStencilState; }

    // These const_casts shouldn't be necessary, but we need to fix the API to accept "const void* pShaderBytecode"
    void SetVertexShader(ComPtr<ID3D11VertexShader> InShader) { m_vertexShader = InShader; }
    void SetPixelShader(ComPtr<ID3D11PixelShader> InShader) { m_pixelShader = InShader; }
    void SetGeometryShader(ComPtr<ID3D11GeometryShader> InShader) { m_geometryShader = InShader; }
    void SetHullShader(ComPtr<ID3D11HullShader> InShader) { m_hullShader = InShader; }
    void SetDomainShader(ComPtr<ID3D11DomainShader> InShader) { m_domainShader = InShader; }

private:
	wchar_t* m_Name = nullptr;

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3D11PixelShader> m_pixelShader;
    ComPtr<ID3D11HullShader> m_hullShader;
    ComPtr<ID3D11DomainShader> m_domainShader;
    ComPtr<ID3D11GeometryShader> m_geometryShader;
    ComPtr<ID3D11InputLayout> m_inputLayout;

    ComPtr<ID3D11BlendState> m_blendState;
    ComPtr<ID3D11DepthStencilState> m_depthStencilState;
    ComPtr<ID3D11RasterizerState> m_rasterizerState;


    float m_blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    UINT m_stencilRef = 0;

    D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

};


namespace States {

    //SamplerStates
   extern ComPtr<ID3D11SamplerState> linearWrapSS;
   extern ComPtr<ID3D11SamplerState> linearClampSS;

   //RasterizeState
   extern ComPtr<ID3D11RasterizerState> solidRS;
   extern ComPtr<ID3D11RasterizerState> wireRS;
   extern ComPtr<ID3D11RasterizerState> solidCCWRS;
   extern ComPtr<ID3D11RasterizerState> wireCCWRS;
   extern ComPtr<ID3D11RasterizerState> postProcessingRS;

   //BlendState
   extern ComPtr<ID3D11BlendState> translucentBS;

   //VertexShaders
   extern ComPtr<ID3D11VertexShader> basicVS;
   extern ComPtr<ID3D11VertexShader> skyboxVS;
   extern ComPtr<ID3D11VertexShader> samplingVS;
   extern ComPtr<ID3D11VertexShader> normalVS;
   extern ComPtr<ID3D11VertexShader> depthOnlyVS;

   //PixelShaders
   extern ComPtr<ID3D11PixelShader> basicPS;
   extern ComPtr<ID3D11PixelShader> skyboxPS;
   extern ComPtr<ID3D11PixelShader> combinePS;
   extern ComPtr<ID3D11PixelShader> normalPS;
   extern ComPtr<ID3D11PixelShader> depthOnlyPS;
   extern ComPtr<ID3D11PixelShader> postEffectsPS;

   //InputLayout
   extern ComPtr<ID3D11InputLayout> basicIL;
   extern ComPtr<ID3D11InputLayout> samplingIL;
   extern ComPtr<ID3D11InputLayout> skyboxIL;
   extern ComPtr<ID3D11InputLayout> postProcessingIL;


   extern GraphicsPSO defaultSolidPSO;
   extern GraphicsPSO defaultWirePSO;

   void InitStates(ComPtr<ID3D11Device>& device);

   //SamplerStates
   void InitSamplerStates(ComPtr<ID3D11Device>& device);


   //SamplerStates
   void InitRasterizerState(ComPtr<ID3D11Device>& device);

   void InitBlendStates(ComPtr<ID3D11Device>& device);

   void InitVertexShaders(ComPtr<ID3D11Device>& device);

   void InitPixelShaders(ComPtr<ID3D11Device>& device);

   void InitInputLayouts(ComPtr<ID3D11Device>& device);

};
