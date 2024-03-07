#include "ImageFilter.h"

void HImageFilter::Initialize(HBaseRenderModule* ParentModule, const wstring vertexShaderName, const wstring pixelShaderName, int width, int height)
{
    m_mesh = make_shared<MeshObject>();
    HRenderingLibrary::MakeSquare(&m_mesh->mesh);
    m_parentRenderModule = ParentModule;
    auto device = m_parentRenderModule->GetDevice();


    HRenderingLibrary::CreateVertexBuffer(device, &m_mesh->mesh, m_mesh->vertexBuffer);
    HRenderingLibrary::CreateIndexBuffer(device, &m_mesh->mesh, m_mesh->indexBuffer);


    HRenderingLibrary::CreateVertexShader(device, m_vertexShader, m_inputLayout, vertexShaderName.c_str(), HRenderingLibrary::GetVSInputLayout());
    HRenderingLibrary::CreatePixelShader(device, m_pixelShader, pixelShaderName.c_str());


    m_pixelConstData.dx = 1.0f / width;
    m_pixelConstData.dy = 1.0f / height;
    HRenderingLibrary::CreateConstantBuffer(device, m_pixelConstData,
        m_mesh->pixelConstantBuffer);


    // Texture sampler
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the Sample State
    device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());


    // Create a rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = false;

    device->CreateRasterizerState(&rastDesc,
        m_rasterizerSate.GetAddressOf());

    // Set the viewport
    ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = float(width);
    m_viewport.Height = float(height);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;

    ComPtr<ID3D11Texture2D> texture;

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = width;
    txtDesc.Height = height;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //  이미지 처리용도
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE |
        D3D11_BIND_RENDER_TARGET |
        D3D11_BIND_UNORDERED_ACCESS;
    txtDesc.MiscFlags = 0;
    txtDesc.CPUAccessFlags = 0;

    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    viewDesc.Format = txtDesc.Format;
    viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipSlice = 0;

    device->CreateTexture2D(&txtDesc, NULL, texture.GetAddressOf());
    device->CreateRenderTargetView(texture.Get(), &viewDesc,
        m_renderTargetView.GetAddressOf());
    device->CreateShaderResourceView(texture.Get(), nullptr,
        m_shaderResourceView.GetAddressOf());


    // 기본 렌더타겟
   SetRenderTargets({ m_renderTargetView });
}

void HImageFilter::Update()
{

}

void HImageFilter::Render()
{
    assert(m_shaderResources.size() > 0);
    assert(m_renderTargets.size() > 0);

    auto context = m_parentRenderModule->GetContext();
    // 어디에 렌더링 할지를 지정
    context->OMSetRenderTargets(UINT(m_renderTargets.size()),
        m_renderTargets.data(), nullptr);
    //float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    //context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    context->RSSetState(m_rasterizerSate.Get());
    context->RSSetViewports(1, &m_viewport);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    context->IASetInputLayout(m_inputLayout.Get());
    context->IASetVertexBuffers(0, 1, m_mesh->vertexBuffer.GetAddressOf(),
        &stride, &offset);
    context->IASetIndexBuffer(m_mesh->indexBuffer.Get(),
        DXGI_FORMAT_R16_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->VSSetShader(m_vertexShader.Get(), 0, 0);
    context->PSSetShader(m_pixelShader.Get(), 0, 0);
    context->PSSetShaderResources(0, UINT(m_shaderResources.size()),
        m_shaderResources.data());
    context->PSSetConstantBuffers(
        0, 1, m_mesh->pixelConstantBuffer.GetAddressOf());
    context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

    context->DrawIndexed(m_mesh->mesh.indices.size(), 0, 0);
}
