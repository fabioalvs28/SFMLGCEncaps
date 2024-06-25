#include "framework.h"

GCTexture::GCTexture()
{
    m_pTextureBuffer = nullptr;
    m_pUploadTexture = nullptr;
    m_cbvSrvUavDescriptorSize = 0;
}

GCTexture::~GCTexture()
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

    SAFE_RELEASE(m_pTextureBuffer);
    SAFE_RELEASE(m_pUploadTexture);

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
}

bool GCTexture::Initialize(const std::string& filePath, GCGraphics* pGraphics, size_t& textureOffset)
{
    //Initializes textures
    std::wstring wideFilePath(filePath.begin(), filePath.end());

    CHECK_FILE(filePath, "Texture not found: " + filePath, "Texture file : " + filePath + " loaded successfully");

    m_cbvSrvUavDescriptorSize = pGraphics->GetRender()->Getmd3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    DirectX::CreateDDSTextureFromFile12(pGraphics->GetRender()->Getmd3dDevice(), pGraphics->GetRender()->GetCommandList(), wideFilePath.c_str(), &m_pTextureBuffer, &m_pUploadTexture);


    if (m_pTextureBuffer == nullptr || m_pUploadTexture == nullptr)
    {
        return false;
    }

    //Heap
    CD3DX12_CPU_DESCRIPTOR_HANDLE handleDescriptor(pGraphics->GetRender()->GetCbvSrvUavSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
    handleDescriptor.Offset(textureOffset, m_cbvSrvUavDescriptorSize);

    //Desc texture
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Format = m_pTextureBuffer->GetDesc().Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = m_pTextureBuffer->GetDesc().MipLevels;
    srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

    m_mipLevels = m_pTextureBuffer->GetDesc().MipLevels;

    pGraphics->GetRender()->Getmd3dDevice()->CreateShaderResourceView(m_pTextureBuffer, &srvDesc, handleDescriptor);

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(pGraphics->GetRender()->GetCbvSrvUavSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
    m_textureAddress.Offset(textureOffset, m_cbvSrvUavDescriptorSize);
    return true;
}