#pragma once

class GCTexture
{
public:
    GCTexture();
    ~GCTexture();

    GC_GRAPHICS_ERROR Initialize(const std::string& filePath, GCGraphics* pGraphics, int& textureOffset);

    inline UINT GetCbvSrvUavDescriptorSize() const { return m_cbvSrvUavDescriptorSize; }
    inline ID3D12Resource* GetTextureBuffer() const { return m_pTextureBuffer; }
    inline ID3D12Resource* GetUploadTexture() const { return m_pUploadTexture; }
    inline CD3DX12_GPU_DESCRIPTOR_HANDLE GetTextureAddress() const { return m_textureAddress; }
    inline UINT GetMipLevels() const { return m_mipLevels; }
    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }

private:
    UINT m_cbvSrvUavDescriptorSize;
    ID3D12Resource* m_pTextureBuffer;
    ID3D12Resource* m_pUploadTexture;
    CD3DX12_GPU_DESCRIPTOR_HANDLE m_textureAddress;
    UINT m_mipLevels;

    int m_height = 0;
    int m_width = 0;
};