#include "CustomRenderModule.h"


void HCustomRenderModule::InitSampler()
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

    
}


void HCustomRenderModule::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC VertexBufferDesc = {};
    VertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    VertexBufferDesc.ByteWidth = sizeof(Vertex) * m_drawingMesh.vertices.size();
    VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    VertexBufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA VertexBufferData = {};
    VertexBufferData.pSysMem = m_drawingMesh.vertices.data();

    m_device->CreateBuffer(&VertexBufferDesc, &VertexBufferData, m_vertexBuffer.GetAddressOf());

    if (m_vertexBuffer.Get() == nullptr)
    {
        cout << "No Vertex Buffer!" << endl;
    }
}
void HCustomRenderModule::CreateIndexBuffer()
{
    D3D11_BUFFER_DESC IndexBufferDesc = {};
    IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    IndexBufferDesc.ByteWidth = sizeof(uint16_t) * m_drawingMesh.indices.size();
    IndexBufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA IndexBufferData = {};
    IndexBufferData.pSysMem = m_drawingMesh.indices.data();

    m_device->CreateBuffer(&IndexBufferDesc, &IndexBufferData, m_indexBuffer.GetAddressOf());


    if (m_indexBuffer.Get() == nullptr)
    {
        cout << "No Index Buffer!" << endl;
    }
}
void HCustomRenderModule::CreateConstantBuffer()
{
    D3D11_BUFFER_DESC ConstBufferDesc = {};
    ConstBufferDesc.ByteWidth = sizeof(m_transformConstData);
    ConstBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    ConstBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ConstBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    ConstBufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA ConstBufferData = {};
    ConstBufferData.pSysMem = &m_transformConstData;

    m_device->CreateBuffer(&ConstBufferDesc, &ConstBufferData, m_transformConstBuffer.GetAddressOf());

    if (m_transformConstBuffer.Get() == nullptr)
    {
        cout << "No Const Buffer!" << endl;
    }
}

void HCustomRenderModule::CreateVertexShader()
{
    D3D11_INPUT_ELEMENT_DESC position;
    D3D11_INPUT_ELEMENT_DESC color;

    position.SemanticName = "POSITION";
    position.SemanticIndex = 0;
    position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    position.InputSlot = 0;
    position.AlignedByteOffset = 0;
    position.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
    position.InstanceDataStepRate = 0;

    color.SemanticName = "COLOR";
    color.SemanticIndex = 0;
    color.Format = DXGI_FORMAT_R32G32B32_FLOAT;
    color.InputSlot = 0;
    color.AlignedByteOffset = 4 * 3;
    color.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
    color.InstanceDataStepRate = 0;

    vector<D3D11_INPUT_ELEMENT_DESC> inputs = { position,color };

    ComPtr<ID3DBlob> VSBlob;
    ComPtr<ID3DBlob> VSErrorBlob;

    HRESULT VShr = D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &VSBlob, &VSErrorBlob);

    if (FAILED(VShr))
    {
        cout << (char*)VSErrorBlob->GetBufferPointer() << endl;
    }

    m_device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, m_vertexShader.GetAddressOf());
    m_device->CreateInputLayout(inputs.data(), UINT(inputs.size()), VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), m_vertexInputLayout.GetAddressOf());
}


void HCustomRenderModule::CreatePixelShader()
{
    ComPtr<ID3DBlob> PSBlob;
    ComPtr<ID3DBlob> PSErrorBlob;

    HRESULT PShr = D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &PSBlob, &PSErrorBlob);

    if (FAILED(PShr))
    {
        cout << (char*)PSErrorBlob->GetBufferPointer() << endl;
    }

    m_device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, m_pixelShader.GetAddressOf());
}
