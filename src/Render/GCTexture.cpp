#include "framework.h"


GCTexture::GCTexture()
{
    m_pTextureBuffer = nullptr;
    m_pUploadTexture = nullptr;
    m_cbvSrvUavDescriptorSize = 0;
}

GCTexture::~GCTexture()
{
    // Free texture from GPU, for create new texture at same place after
    if (m_pTextureBuffer != nullptr)
    {
        m_pTextureBuffer->Release();
        m_pTextureBuffer = nullptr;
    }

    if (m_pUploadTexture != nullptr)
    {
        delete m_pUploadTexture;
        m_pUploadTexture = nullptr;
    }

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
}

bool GCTexture::Initialize(const std::string& filePath, GCGraphics* pGraphics)
{

    std::wstring wideFilePath(filePath.begin(), filePath.end());

    if (_waccess(wideFilePath.c_str(), 0) == 0)
        OutputDebugString((L"Shader not found: " + wideFilePath + L"\n").c_str());


    m_cbvSrvUavDescriptorSize = pGraphics->GetRender()->Getmd3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


    DirectX::CreateDDSTextureFromFile12(pGraphics->GetRender()->Getmd3dDevice(), pGraphics->GetRender()->GetCommandList(), wideFilePath.c_str(), &m_pTextureBuffer, &m_pUploadTexture);
    if (m_pTextureBuffer == nullptr || m_pUploadTexture == nullptr)
        return false;

    // Heap
    CD3DX12_CPU_DESCRIPTOR_HANDLE handleDescriptor(pGraphics->GetRender()->GetCbvSrvUavSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
    handleDescriptor.Offset(pGraphics->GetTextureId(), m_cbvSrvUavDescriptorSize);

    // Desc texture
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = m_pTextureBuffer->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = m_pTextureBuffer->GetDesc().MipLevels;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

    pGraphics->GetRender()->Getmd3dDevice()->CreateShaderResourceView(m_pTextureBuffer, &srvDesc, handleDescriptor);

    //getGPU pour les dessiner
    // Manager

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(pGraphics->GetRender()->GetCbvSrvUavSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
    m_textureAddress.Offset(pGraphics->GetTextureId(), m_cbvSrvUavDescriptorSize);
    return true;
}