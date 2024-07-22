#pragma once


class GCShaderUploadBufferBase
{
public:
    GCShaderUploadBufferBase() : m_pUpload(nullptr), m_data(nullptr), m_elementByteSize(0), m_isConstantBuffer(false), m_isUsed(false), m_framesSinceLastUse(0) {}
    virtual ~GCShaderUploadBufferBase()
    {
        if (m_pUpload)
        {
            m_pUpload->Unmap(0, nullptr);
        }
        m_data = nullptr;
    }

    ID3D12Resource* Resource() const
    {
        return m_pUpload;
    }

    virtual void CopyData(int elementIndex, const GCSHADERCB& data) = 0;

    bool m_isUsed;
    int m_framesSinceLastUse;

protected:
    ID3D12Resource* m_pUpload;
    BYTE* m_data;
    UINT m_elementByteSize;
    bool m_isConstantBuffer;

    UINT CalcConstantBufferByteSize(UINT byteSize)
    {
        return (byteSize + 255) & ~255;
    }
};

template<typename T>
class GCShaderUploadBuffer : public GCShaderUploadBufferBase 
{
public:
    GCShaderUploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) 
    {
        m_isConstantBuffer = isConstantBuffer;
        m_elementByteSize = sizeof(T);

        if (isConstantBuffer) 
        {
            m_elementByteSize = CalcConstantBufferByteSize(sizeof(T));
        }

        CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize * elementCount);
        CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_pUpload));

        m_pUpload->Map(0, nullptr, reinterpret_cast<void**>(&m_data));
    }

    void CopyData(int elementIndex, const GCSHADERCB& data) override {
        memcpy(&m_data[elementIndex * m_elementByteSize], &data, sizeof(T));
    }
};


class GCUploadBufferBase
{
public:
    GCUploadBufferBase() : m_pUpload(nullptr), m_data(nullptr), m_elementByteSize(0), m_isUsed(false), m_framesSinceLastUse(0) {}
    virtual ~GCUploadBufferBase()
    {
        if (m_pUpload)
        {
            m_pUpload->Unmap(0, nullptr);
        }
        m_data = nullptr;
    }

    ID3D12Resource* Resource() const
    {
        return m_pUpload;
    }

    virtual void CopyData(int elementIndex, const void* data, size_t dataSize) = 0;

    bool m_isUsed;
    int m_framesSinceLastUse;

protected:
    ID3D12Resource* m_pUpload;
    BYTE* m_data;
    UINT m_elementByteSize;
    bool m_isConstantBuffer;

    UINT CalcConstantBufferByteSize(UINT byteSize)
    {
        return (byteSize + 255) & ~255;
    }
};

template<typename T>
class GCUploadBuffer : public GCUploadBufferBase
{
public:
    GCUploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer)
    {
        m_isConstantBuffer = isConstantBuffer;
        m_elementByteSize = sizeof(T);

        if (isConstantBuffer)
        {
            m_elementByteSize = CalcConstantBufferByteSize(sizeof(T));
        }

        CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_elementByteSize * elementCount);
        CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

        device->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&m_pUpload));

        m_pUpload->Map(0, nullptr, reinterpret_cast<void**>(&m_data));
    }

    void CopyData(int elementIndex, const void* data, size_t dataSize) override {
        memcpy(&m_data[elementIndex * m_elementByteSize], data, dataSize);
    }
};


