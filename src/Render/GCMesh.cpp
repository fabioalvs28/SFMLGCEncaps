#include "framework.h"

ID3D12Resource* GCMesh::CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    ID3D12Resource* uploadBuffer)
{
    ID3D12Resource* defaultBuffer;

    // Create the actual default buffer resource.
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC resDesc(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&defaultBuffer));

    // In order to copy CPU memory data into our default buffer, we need to create
    // an intermediate upload heap.

    CD3DX12_HEAP_PROPERTIES heapPropUp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resDesc2(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    device->CreateCommittedResource(
        &heapPropUp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer));


    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
    // will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
    // the intermediate upload heap data will be copied to mBuffer.

    CD3DX12_RESOURCE_BARRIER ResBarrier(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
        D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    cmdList->ResourceBarrier(1, &ResBarrier);
    UpdateSubresources<1>(cmdList, defaultBuffer, uploadBuffer, 0, 0, 1, &subResourceData);
    CD3DX12_RESOURCE_BARRIER ResBarrier2(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
        D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
    cmdList->ResourceBarrier(1, &ResBarrier2);


    // Note: uploadBuffer has to be kept alive after the above function calls because
    // the command list has not been executed yet that performs the actual copy.
    // The caller can Release the uploadBuffer after it knows the copy has been executed.


    return defaultBuffer;
}

GCMesh::GCMesh() 
{
    m_pRender = nullptr;
    m_pBufferGeometryData = nullptr;
    m_flagEnabledBits = 0;
}

GCMesh::~GCMesh()
{
    if (m_pBufferGeometryData)
    {
        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferCPU);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferCPU);

        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferGPU);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferGPU);

        SAFE_RELEASE(m_pBufferGeometryData->VertexBufferUploader);
        SAFE_RELEASE(m_pBufferGeometryData->IndexBufferUploader);

        m_pBufferGeometryData->VertexByteStride = 0;
        m_pBufferGeometryData->VertexBufferByteSize = 0;
        m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
        m_pBufferGeometryData->IndexBufferByteSize = 0;
        m_pBufferGeometryData->IndexCount = 0;

        SAFE_DELETE(m_pBufferGeometryData);
    }
}

void GCMesh::Initialize(GCRender* pRender, GCGeometry* pGeometry) {
    m_pRender = pRender;
    UploadGeometryData(pGeometry);
}

void GCMesh::UploadGeometryData(GCGeometry* pGeometry) {
    std::vector<float> vertexData;

    m_flagEnabledBits = pGeometry->m_flagEnabledBits;

    size_t vertexSize = 0;
    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) vertexSize += 3; // 3 floats pour la position
    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) vertexSize += 4; // 4 floats pour la couleur
    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) vertexSize += 2; // 2 floats pour les UV
    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) vertexSize += 3; // 3 floats pour la normale

    vertexData.reserve(pGeometry->pos.size() * vertexSize);

    for (size_t i = 0; i < pGeometry->pos.size(); ++i) {
        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) {
            vertexData.push_back(pGeometry->pos[i].x);
            vertexData.push_back(pGeometry->pos[i].y);
            vertexData.push_back(pGeometry->pos[i].z);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) {
            vertexData.push_back(pGeometry->color[i].x);
            vertexData.push_back(pGeometry->color[i].y);
            vertexData.push_back(pGeometry->color[i].z);
            vertexData.push_back(pGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) {
            vertexData.push_back(pGeometry->uv[i].x);
            vertexData.push_back(pGeometry->uv[i].y);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(pGeometry->indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);

    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), pGeometry->indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), vertexData.data(), vbByteSize, m_pBufferGeometryData->VertexBufferUploader);
    m_pBufferGeometryData->IndexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), pGeometry->indices.data(), ibByteSize, m_pBufferGeometryData->IndexBufferUploader);

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pGeometry->indices.size());
}