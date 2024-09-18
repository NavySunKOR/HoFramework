#include "PSO.h"
#include "RenderingLibrary.h"

HGraphicsPSO::HGraphicsPSO()
{
}


namespace States {

    ComPtr<ID3D11SamplerState> linearWrapSS;
    ComPtr<ID3D11SamplerState> linearClampSS;

    ComPtr<ID3D11RasterizerState> solidRS;
    ComPtr<ID3D11RasterizerState> wireRS;
    ComPtr<ID3D11RasterizerState> solidCCWRS;
    ComPtr<ID3D11RasterizerState> wireCCWRS;


    ComPtr<ID3D11BlendState> translucentBS;

    // Shaders
    ComPtr<ID3D11VertexShader> basicVS;
    ComPtr<ID3D11VertexShader> skyboxVS;
    ComPtr<ID3D11VertexShader> postProcessBaseVS;
    ComPtr<ID3D11VertexShader> depthOnlyVS;

    ComPtr<ID3D11PixelShader> basicPS;
    ComPtr<ID3D11PixelShader> skyboxPS;
    ComPtr<ID3D11PixelShader> postProcessBasePS;
    ComPtr<ID3D11PixelShader> depthOnlyPS;
    ComPtr<ID3D11PixelShader> blurXPPPS;
    ComPtr<ID3D11PixelShader> blurYPPPS;


    ComPtr<ID3D11InputLayout> basicIL;
    ComPtr<ID3D11InputLayout> skyboxIL;
    ComPtr<ID3D11InputLayout> postProcessBaseIL;

    ComPtr<ID3D11DepthStencilState> depthStencilState;

    HGraphicsPSO basicSolidPSO;
    HGraphicsPSO basicWirePSO;

    HGraphicsPSO postProcessBasePSO;
    HGraphicsPSO skyboxPSO;
};


//PSO를 캐싱한다면 여기서 캐싱함.
//언리얼 엔진같이 매우 Versatile 하게 사용할려면 DX12의 Sample을 보고 만들어야 되며, 쉐이더 또한 머터리얼 만든 것에다가 입혀서 사용해야함.
//두번째로 머터리얼 생성 시, 거기에 들어갈 쉐이더 패스나 이런것 또한 파일로 관리해야 되는데 그 작업하다가 그래픽 구현을 못하는 수가 있음.
void States::InitStates(ComPtr<ID3D11Device>& device)
{
    InitSamplerStates(device);
    InitRasterizerState(device);
    InitBlendStates(device);
    InitVertexShaders(device);
    InitPixelShaders(device);


    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; // false
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    device->CreateDepthStencilState(&depthStencilDesc, depthStencilState.GetAddressOf());

    basicSolidPSO.m_vertexShader = basicVS;
    basicSolidPSO.m_pixelShader = basicPS;
    basicSolidPSO.m_inputLayout = basicIL;
    basicSolidPSO.m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    basicSolidPSO.m_rasterizerState = solidRS;
    basicSolidPSO.m_depthStencilState = depthStencilState;
    basicSolidPSO.m_samplerState = linearClampSS;

    basicWirePSO = basicSolidPSO;
    basicWirePSO.m_rasterizerState = wireRS;

    //skyboxPSO = basicSolidPSO;
    //skyboxPSO.m_vertexShader = skyboxVS;
    //skyboxPSO.m_pixelShader = skyboxPS;
    //skyboxPSO.m_inputLayout = skyboxIL;

    skyboxPSO.m_vertexShader = skyboxVS;
    skyboxPSO.m_pixelShader = skyboxPS;
    skyboxPSO.m_inputLayout = skyboxIL;
    skyboxPSO.m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    skyboxPSO.m_rasterizerState = solidRS;
    skyboxPSO.m_depthStencilState = depthStencilState;
    skyboxPSO.m_samplerState = linearWrapSS;

    postProcessBasePSO = basicSolidPSO;
    postProcessBasePSO.m_vertexShader = postProcessBaseVS;
    postProcessBasePSO.m_pixelShader = postProcessBasePS;

    postProcessBasePSO.m_samplerState = linearClampSS;
    postProcessBasePSO.m_rasterizerState = solidRS;
}


void HGraphicsPSO::operator=(const HGraphicsPSO& pso) {

    m_vertexShader = pso.m_vertexShader;
    m_pixelShader = pso.m_pixelShader;
    m_hullShader = pso.m_hullShader;
    m_domainShader = pso.m_domainShader;
    m_geometryShader = pso.m_geometryShader;
    m_inputLayout = pso.m_inputLayout;
    m_blendState = pso.m_blendState;
    m_depthStencilState = pso.m_depthStencilState;
    m_rasterizerState = pso.m_rasterizerState;

    for (int i = 0; i < 4; i++)
        m_blendFactor[i] = pso.m_blendFactor[i];
    m_primitiveTopology = pso.m_primitiveTopology;

    m_stencilRef = pso.m_stencilRef;
}

void States::InitSamplerStates(ComPtr<ID3D11Device>& device)
{
    //Linear
    D3D11_SAMPLER_DESC linearClampSampDesc;
    ZeroMemory(&linearClampSampDesc, sizeof(linearClampSampDesc));
    linearClampSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    linearClampSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    linearClampSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    linearClampSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    linearClampSampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    linearClampSampDesc.MinLOD = 0;
    linearClampSampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the Sample State
    device->CreateSamplerState(&linearClampSampDesc, linearClampSS.GetAddressOf());


    D3D11_SAMPLER_DESC linearWrapSampDesc;
    linearWrapSampDesc = linearClampSampDesc;
    linearWrapSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    linearWrapSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    linearWrapSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


    // Create the Sample State
    device->CreateSamplerState(&linearWrapSampDesc, linearWrapSS.GetAddressOf());

}

void States::InitRasterizerState(ComPtr<ID3D11Device>& device)
{

    D3D11_RASTERIZER_DESC rastSolidDesc;
    {
        ZeroMemory(&rastSolidDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
        rastSolidDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rastSolidDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
        rastSolidDesc.FrontCounterClockwise = false;
        rastSolidDesc.DepthClipEnable = true;

        device->CreateRasterizerState(&rastSolidDesc,
            solidRS.GetAddressOf());
    }

    {
        D3D11_RASTERIZER_DESC rastSolidCCWDesc;
        rastSolidCCWDesc = rastSolidDesc;
        rastSolidCCWDesc.FrontCounterClockwise = true;

        device->CreateRasterizerState(&rastSolidCCWDesc,
            solidCCWRS.GetAddressOf());
    }


    D3D11_RASTERIZER_DESC rastWireDesc;
    {
        rastWireDesc = rastSolidDesc;
        rastWireDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        device->CreateRasterizerState(&rastWireDesc,
            wireRS.GetAddressOf());
    }

    {
        D3D11_RASTERIZER_DESC rastWireCCWDesc;
        rastWireCCWDesc = rastWireDesc;
        rastWireCCWDesc.FrontCounterClockwise = true;
        device->CreateRasterizerState(&rastWireCCWDesc,
            wireCCWRS.GetAddressOf());
    }
}

void States::InitBlendStates(ComPtr<ID3D11Device>& device)
{
    //Translucent 처리
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable = true; // MSAA
    blendDesc.IndependentBlendEnable = false;
    // 개별 RenderTarget에 대해서 설정 (최대 8개)
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    // 필요하면 RGBA 각각에 대해서도 조절 가능
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    device->CreateBlendState(&blendDesc, translucentBS.GetAddressOf());
      
}

//쉐이더에 사용할 
void States::InitVertexShaders(ComPtr<ID3D11Device>& device)
{

    //basicVS
    HRenderingLibrary::CreateVertexShader(device, basicVS, basicIL, L"./Shaders/Lit/LitVertexShader.hlsl", "main", HRenderingLibrary::GetVSInputLayout());
    //skyboxVS
    HRenderingLibrary::CreateVertexShader(device, skyboxVS, skyboxIL, L"./Shaders/Skybox/VertexShaderSkybox.hlsl", "main", HRenderingLibrary::GetVSInputLayout());
    //postProcessBaseVS
    HRenderingLibrary::CreateVertexShader(device, postProcessBaseVS, postProcessBaseIL, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", "main", HRenderingLibrary::GetVSInputLayout());


}

void States::InitPixelShaders(ComPtr<ID3D11Device>& device)
{
    HRenderingLibrary::CreatePixelShader(device, basicPS, L"./Shaders/Lit/LitPixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, skyboxPS, L"./Shaders/Skybox/PixelShaderSkybox.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, postProcessBasePS, L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, blurXPPPS, L"./Shaders/ImageFilters/Blur/BlurXPixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, blurYPPPS, L"./Shaders/ImageFilters/Blur/BlurYPixelShader.hlsl", "main");
}
