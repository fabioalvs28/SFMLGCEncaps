#pragma once
//
//#include "d3dUtil.h"

// Nouveau Upload Buffer pour les derivés de ShaderCB
class SUploadBufferBase {
public:
    SUploadBufferBase() : m_pUpload(nullptr), m_data(nullptr), m_elementByteSize(0), m_isConstantBuffer(false) {}
    virtual ~SUploadBufferBase() {
        if (m_pUpload) {
            m_pUpload->Unmap(0, nullptr);
        }
        m_data = nullptr;
    }

    ID3D12Resource* Resource() const {
        return m_pUpload.Get();
    }

    virtual void CopyData(int elementIndex, const ShaderCB& data) = 0;

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> m_pUpload;
    BYTE* m_data;
    UINT m_elementByteSize;
    bool m_isConstantBuffer;

    UINT CalcConstantBufferByteSize(UINT byteSize) {
        return (byteSize + 255) & ~255;
    }
};

template<typename T>
class SUploadBuffer : public SUploadBufferBase {
public:
    SUploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) {
        m_isConstantBuffer = isConstantBuffer;
        m_elementByteSize = sizeof(T);

        if (isConstantBuffer) {
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

    void CopyData(int elementIndex, const ShaderCB& data) override {
        memcpy(&m_data[elementIndex * m_elementByteSize], &data, sizeof(T));
    }
};


// Old Upload Buffer - Encore Utilisé pour la caméra
template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
        mIsConstantBuffer(isConstantBuffer)
    {
        mElementByteSize = sizeof(T);

        // Constant buffer elements need to be multiples of 256 bytes.
        // This is because the hardware can only view constant data 
        // at m*256 byte offsets and of n*256 byte lengths. 
        // typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
        // UINT64 OffsetInBytes; // multiple of 256
        // UINT   SizeInBytes;   // multiple of 256
        // } D3D12_CONSTANT_BUFFER_VIEW_DESC;
        if (isConstantBuffer)
            mElementByteSize = CalcConstantBufferByteSize(sizeof(T));

        CD3DX12_RESOURCE_DESC ResDesc(CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount));
        CD3DX12_HEAP_PROPERTIES prop(D3D12_HEAP_TYPE_UPLOAD);
        device->CreateCommittedResource(
            &prop,
            D3D12_HEAP_FLAG_NONE,
            &ResDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&mUploadBuffer));

        mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));

        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;

    virtual ~UploadBuffer()
    {
        if (mUploadBuffer != nullptr)
            mUploadBuffer->Unmap(0, nullptr);

        mMappedData = nullptr;
    }

    ID3D12Resource* Resource()const
    {
        return mUploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&mMappedData[elementIndex * mElementByteSize], &data, sizeof(T));
    }
    UINT CalcConstantBufferByteSize(UINT byteSize)
    {
        // Constant buffers must be a multiple of the minimum hardware
        // allocation size (usually 256 bytes).  So round up to nearest
        // multiple of 256.  We do this by adding 255 and then masking off
        // the lower 2 bytes which store all bits < 256.
        // Example: Suppose byteSize = 300.
        // (300 + 255) & ~255
        // 555 & ~255
        // 0x022B & ~0x00ff
        // 0x022B & 0xff00
        // 0x0200
        // 512
        return (byteSize + 255) & ~255;
    }
private:
    Microsoft::WRL::ComPtr<ID3D12Resource> mUploadBuffer;
    BYTE* mMappedData = nullptr;

    UINT mElementByteSize = 0;
    bool mIsConstantBuffer = false;
};