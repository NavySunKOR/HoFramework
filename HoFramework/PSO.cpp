#include "PSO.h"
#include "RenderingLibrary.h"

GraphicsPSO::GraphicsPSO()
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

};


//PSO�� ĳ���Ѵٸ� ���⼭ ĳ����.
//�𸮾� �������� �ſ� Versatile �ϰ� ����ҷ��� DX12�� Sample�� ���� ������ �Ǹ�, ���̴� ���� ���͸��� ���� �Ϳ��ٰ� ������ ����ؾ���.
//�ι�°�� ���͸��� ���� ��, �ű⿡ �� ���̴� �н��� �̷��� ���� ���Ϸ� �����ؾ� �Ǵµ� �� �۾��ϴٰ� �׷��� ������ ���ϴ� ���� ����.
void States::InitStates(ComPtr<ID3D11Device>& device)
{
    InitSamplerStates(device);
    InitRasterizerState(device);
    InitBlendStates(device);
    InitVertexShaders(device);
    InitPixelShaders(device);


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
    linearClampSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    linearClampSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    linearClampSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


    // Create the Sample State
    device->CreateSamplerState(&linearClampSampDesc, linearWrapSS.GetAddressOf());


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
    //Translucent ó��
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(blendDesc));
    blendDesc.AlphaToCoverageEnable = true; // MSAA
    blendDesc.IndependentBlendEnable = false;
    // ���� RenderTarget�� ���ؼ� ���� (�ִ� 8��)
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_BLEND_FACTOR;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_BLEND_FACTOR;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    // �ʿ��ϸ� RGBA ������ ���ؼ��� ���� ����
    blendDesc.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;

    device->CreateBlendState(&blendDesc, translucentBS.GetAddressOf());
      
}

//���̴��� ����� 
void States::InitVertexShaders(ComPtr<ID3D11Device>& device)
{

    //basicVS
    HRenderingLibrary::CreateVertexShader(device, basicVS, basicIL, L"./Shaders/Lit/LitVertexShader.hlsl", "main", HRenderingLibrary::GetVSInputLayout());
    //skyboxVS
    HRenderingLibrary::CreateVertexShader(device, skyboxVS, skyboxIL, L"./Shaders/Skybox/VertexShaderSkybox.hlsl", "main", HRenderingLibrary::GetVSInputLayout());
    //postProcessBaseVS
    HRenderingLibrary::CreateVertexShader(device, skyboxVS, skyboxIL, L"./Shaders/ImageFilters/Base/ImageVertexShader.hlsl", "main", HRenderingLibrary::GetVSInputLayout());


}

void States::InitPixelShaders(ComPtr<ID3D11Device>& device)
{
    HRenderingLibrary::CreatePixelShader(device, basicPS, L"./Shaders/Lit/LitPixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, skyboxPS, L"./Shaders/Skybox/PixelShaderSkybox.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, postProcessBasePS, L"./Shaders/ImageFilters/Base/ImagePixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, basicPS, L"./Shaders/Skybox/PixelShaderSkybox.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, blurXPPPS, L"./Shaders/ImageFilters/Blur/BlurXPixelShader.hlsl", "main");
    HRenderingLibrary::CreatePixelShader(device, blurYPPPS, L"./Shaders/ImageFilters/Blur/BlurYPixelShader.hlsl", "main");
}
