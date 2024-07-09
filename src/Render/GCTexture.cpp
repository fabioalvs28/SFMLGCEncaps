#include "pch.h"

GCTexture::GCTexture()
{
    m_pTextureBuffer = nullptr;
    m_pUploadTexture = nullptr;
    m_cbvSrvUavDescriptorSize = 0;
}

GCTexture::~GCTexture()
{
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

    SAFE_RELEASE(m_pTextureBuffer);
    SAFE_RELEASE(m_pUploadTexture);

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(D3D12_DEFAULT);
}

GC_GRAPHICS_ERROR GCTexture::Initialize(const std::string& filePath, GCGraphics* pGraphics, size_t& textureOffset)
{
    //Initializes textures
    std::wstring wideFilePath(filePath.begin(), filePath.end());

    if (!CHECK_POINTERSNULL("Graphics ptr is not null", "Graphic pointer is null", pGraphics))
        return GCRENDER_ERROR_TEXTURE_CREATION_FAILED;
    if (!CHECK_FILE(filePath, "Texture not found: " + filePath, "Texture file : " + filePath + " loaded successfully"))
        return GCRENDER_ERROR_TEXTURE_CREATION_FAILED;

    m_cbvSrvUavDescriptorSize = pGraphics->GetRender()->GetRenderResources()->Getmd3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    DirectX::CreateDDSTextureFromFile12(pGraphics->GetRender()->GetRenderResources()->Getmd3dDevice(), pGraphics->GetRender()->GetRenderResources()->GetCommandList(), wideFilePath.c_str(), &m_pTextureBuffer, &m_pUploadTexture);

    if (m_pTextureBuffer == nullptr || m_pUploadTexture == nullptr)
    {
        return GCRENDER_ERROR_POINTER_NULL;
    }

    //Heap
    CD3DX12_CPU_DESCRIPTOR_HANDLE handleDescriptor(pGraphics->GetRender()->GetRenderResources()->GetCbvSrvUavSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
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

    pGraphics->GetRender()->GetRenderResources()->Getmd3dDevice()->CreateShaderResourceView(m_pTextureBuffer, &srvDesc, handleDescriptor);

    m_textureAddress = CD3DX12_GPU_DESCRIPTOR_HANDLE(pGraphics->GetRender()->GetRenderResources()->GetCbvSrvUavSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
    m_textureAddress.Offset(textureOffset, m_cbvSrvUavDescriptorSize);

    if (!CHECK_POINTERSNULL("Texture buffers are not null", "Texture buffers are null", m_pTextureBuffer, m_pUploadTexture))
        return GCRENDER_ERROR_POINTER_NULL;

    return GCRENDER_SUCCESS_OK;
}