#include "BaseRenderModule.h"
#include "Application.h"
#include "imgui.h"

using namespace std;
#pragma comment(lib,"d3d11.lib")


bool HBaseRenderModule::Initialize(Application* pAppContext)
{
    // 여기서 생성하는 것들

    m_AppContext = pAppContext;

    if (m_AppContext == nullptr)
        return false;

    if (!InitDeviceAndContext())
        return false;

    if (!InitSwapChain())
        return false;

    if (!InitRasterizerState())
        return false;

    if (!InitRenderTargetView())
        return false;

    if (!InitDepthBuffer())
        return false;

    if (!InitDepthStencil())
        return false;

    if (!InitSampler())
        return false;

    SetViewport();


    m_MainView.SetApplication(m_AppContext);

    m_IsInitialized = true;

    return true;
}

void HBaseRenderModule::Update() {
    if (m_PrevIsWireframe != m_CurrentIsWireframe)
    {
        m_PrevIsWireframe = m_CurrentIsWireframe;
        InitRasterizerState();
    }

};
void HBaseRenderModule::Render() {

    m_context->RSSetViewports(1, &m_screenViewport);
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
    m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    //이거 매 프레임마다 클리어 해줄 것.
    if (m_depthStencilView)
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
    else
        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
};

bool HBaseRenderModule::InitSampler()
{
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the Sample State
    m_device->CreateSamplerState(&sampDesc, m_SamplerState.GetAddressOf());
    return (m_SamplerState.Get()) ? true : false;
}

bool HBaseRenderModule::InitDeviceAndContext()
{
    m_DriverType = D3D_DRIVER_TYPE_HARDWARE;

    // m_device와 m_context 생성

    m_CreateDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    m_CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    if (FAILED(D3D11CreateDevice(
        nullptr,    // Specify nullptr to use the default adapter.
        m_DriverType, // Create a device using the hardware graphics driver.
        0, // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
        m_CreateDeviceFlags, // Set debug and Direct2D compatibility flags.
        m_FeatureLevels,     // List of feature levels this app can support.
        ARRAYSIZE(m_FeatureLevels), // Size of the list above.
        D3D11_SDK_VERSION,     // Always set this to D3D11_SDK_VERSION for
                               // Microsoft Store apps.
        device.GetAddressOf(), // Returns the Direct3D device created.
        &m_UsingFeatureLevel,         // Returns feature level of device created.
        context.GetAddressOf() // Returns the device immediate context.
    ))) {
        cout << "D3D11CreateDevice() failed." << endl;
        return false;
    }

    if (m_UsingFeatureLevel != D3D_FEATURE_LEVEL_11_0) {
        cout << "D3D Feature Level 11 unsupported." << endl;
        return false;
    }

    // 참고: Immediate vs deferred context
    // A deferred context is primarily used for multithreading and is not
    // necessary for a single-threaded application.
    // https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro#deferred-context

    // 4X MSAA 지원하는지 확인
    UINT numQualityLevels; // Resize 처리를 위해 멤버 변수로 변경
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4,
        &numQualityLevels);
    if (numQualityLevels <= 0) {
        cout << "MSAA not supported." << endl;
        m_MSAAQuality = 0;
    }
    else
    {
        m_MSAAQuality = numQualityLevels;
    }

    if (FAILED(device.As(&m_device))) {
        cout << "device.AS() failed." << endl;
        return false;
    }

    if (FAILED(context.As(&m_context))) {
        cout << "context.As() failed." << endl;
        return false;
    }

    return true;
}
bool HBaseRenderModule::InitSwapChain()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = m_AppContext->GetScreenWidth();   // set the back buffer width
    sd.BufferDesc.Height = m_AppContext->GetScreenHeight(); // set the back buffer height
    sd.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; // floats로 변경 <-HDRI 랑 아닌거 구분 하려면 나중에 바꿔줌
    sd.BufferCount = 2;                                // Double-buffering
    sd.BufferDesc.RefreshRate.Numerator = 120;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used
    sd.OutputWindow = m_AppContext->GetMainWindow();      // the window to be used
    sd.Windowed = TRUE;                  // windowed/full-screen mode
    sd.Flags =
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if (m_MSAAQuality > 0) {
        sd.SampleDesc.Count = 4; // how many multisamples
        sd.SampleDesc.Quality = m_MSAAQuality - 1;
    }
    else {
        sd.SampleDesc.Count = 1; // how many multisamples
        sd.SampleDesc.Quality = 0;
    }

    if (FAILED(D3D11CreateDeviceAndSwapChain(
        0, // Default adapter
        m_DriverType,
        0, // No software device
        m_CreateDeviceFlags, m_FeatureLevels, 1, D3D11_SDK_VERSION, &sd,
        m_swapChain.GetAddressOf(), m_device.GetAddressOf(), &m_UsingFeatureLevel,
        m_context.GetAddressOf()))) {
        cout << "D3D11CreateDeviceAndSwapChain() failed." << endl;
        return false;
    }
    return true;
}
bool HBaseRenderModule::InitRasterizerState()
{
    D3D11_RASTERIZER_DESC baseRastDessc;
    ZeroMemory(&baseRastDessc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
    baseRastDessc.FillMode = (m_CurrentIsWireframe)? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
    baseRastDessc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    baseRastDessc.FrontCounterClockwise = false;
    baseRastDessc.DepthClipEnable = true; // <- zNear, zFar 확인에 필요

    D3D11_RASTERIZER_DESC solidRastDessc = baseRastDessc;

    m_device->CreateRasterizerState(&solidRastDessc, m_RasterizerState.GetAddressOf());

    if ((m_RasterizerState.Get()))
    {
        m_context->RSSetState(m_RasterizerState.Get());
        return true;
    }
    else
    {
        return false;
    }
}
bool HBaseRenderModule::InitRenderTargetView()
{

    //초기화
    if(m_renderTargetView)
        m_renderTargetView.Reset();

    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    if (backBuffer) {
        m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());

        D3D11_TEXTURE2D_DESC desc;
        backBuffer->GetDesc(&desc);
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.MiscFlags = 0;

        if (FAILED(m_device->CreateTexture2D(&desc, nullptr,
            m_tempTexture.GetAddressOf()))) {
            cout << "Failed()" << endl;
        }

       HRESULT Res =  m_device->CreateShaderResourceView(m_tempTexture.Get(), nullptr, m_renderTargetResourceView.GetAddressOf());
       if (FAILED(Res))
       {
           return false;
       }
    }
    else {
        std::cout << "CreateRenderTargetView() failed." << std::endl;
        return false;
    }

    return true;
}
bool HBaseRenderModule::InitDepthBuffer()
{
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    depthStencilBufferDesc.Width = m_AppContext->GetScreenWidth();
    depthStencilBufferDesc.Height = m_AppContext->GetScreenHeight();
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    if (m_MSAAQuality > 0) {
        depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = m_MSAAQuality - 1;
    }
    else {
        depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
        depthStencilBufferDesc.SampleDesc.Quality = 0;
    }
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    if (FAILED(m_device->CreateTexture2D(&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf()))) {
        std::cout << "CreateTexture2D() failed." << std::endl;
        return false;
    }
    if (FAILED(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), 0, &m_depthStencilView))) {
        std::cout << "CreateDepthStencilView() failed." << std::endl;
        return false;
    }

    return true;
}
bool HBaseRenderModule::InitDepthStencil()
{
    ///다시 호출 될 일이 있으므로 여기서 리셋한다.
    if (m_depthStencilState)
        m_depthStencilState.Reset();

    //// Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; // false
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    if (FAILED(m_device->CreateDepthStencilState(&depthStencilDesc, m_depthStencilState.GetAddressOf()))) {
        cout << "CreateDepthStencilState() failed." << endl;
        return false;
    }

    //스텐실을 사용한다면 스텐실 버퍼 초기화
    m_context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
 
    return true;
}
void HBaseRenderModule::SetViewport()
{
    ZeroMemory(&m_screenViewport, sizeof(D3D11_VIEWPORT));
    m_screenViewport.TopLeftX = 0;
    m_screenViewport.TopLeftY = 0;
    m_screenViewport.Width = float(m_AppContext->GetScreenWidth());
    m_screenViewport.Height = float(m_AppContext->GetScreenHeight());
    m_screenViewport.MinDepth = 0.0f;
    m_screenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

}

void HBaseRenderModule::ResizeWindow()
{
    if (m_swapChain)
    {
        m_swapChain->ResizeBuffers(0, m_AppContext->GetScreenWidth(), m_AppContext->GetScreenHeight(), DXGI_FORMAT_UNKNOWN, 0);
        InitRenderTargetView();
        InitDepthBuffer();
        InitDepthStencil();
        SetViewport();
    }

}
