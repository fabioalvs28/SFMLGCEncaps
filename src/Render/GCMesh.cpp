#include "pch.h"

ID3D12Resource* GCMesh::CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    ID3D12Resource** uploadBuffer) 
{
    ID3D12Resource* defaultBuffer = nullptr;

    // Create the default buffer resource.
    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC resDesc(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    HRESULT hr = device->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&defaultBuffer)
    );

    if (FAILED(hr))
    {
        return nullptr;  // Handle failure
    }

    // Create the upload buffer resource.
    CD3DX12_HEAP_PROPERTIES heapPropUp(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resDesc2(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
    hr = device->CreateCommittedResource(
        &heapPropUp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc2,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(uploadBuffer)
    );

    if (FAILED(hr))
    {
        defaultBuffer->Release();
        return nullptr;  // Handle failure
    }

    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Transition the default buffer to the COPY_DEST state
    CD3DX12_RESOURCE_BARRIER ResBarrier(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
    cmdList->ResourceBarrier(1, &ResBarrier);

    // Copy the data from the upload buffer to the default buffer
    UpdateSubresources(cmdList, defaultBuffer, *uploadBuffer, 0, 0, 1, &subResourceData);

    // Transition the default buffer to the GPU_READ state
    CD3DX12_RESOURCE_BARRIER ResBarrier2(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
    cmdList->ResourceBarrier(1, &ResBarrier2);

    // Return the default buffer
    return defaultBuffer;
}


GCMesh::GCMesh()
{
    m_pRender = nullptr;
    m_pBufferGeometryData = nullptr;
    m_flagEnabledBits = 0;
    m_geoAmount = 0;
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

bool GCMesh::Initialize(GCRender* pRender, GCGeometry* pGeometry, int& flagEnabledBits) 
{
    CHECK_POINTERSNULL("Graphics Initialized with window sucessfully", "Can't initialize Graphics, Window is empty", pRender);
    m_pRender = pRender;
    UploadGeometryData(pGeometry, flagEnabledBits);

    return true;
}

void GCMesh::UploadGeometryData(GCGeometry* pGeometry, int& flagEnabledBits) {
    m_flagEnabledBits = flagEnabledBits;

    std::vector<float> vertexData;
    size_t vertexSize = 0;

    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) vertexSize += 3; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) vertexSize += 2; 
    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) vertexSize += 3; 

    vertexData.reserve(pGeometry->pos.size() * vertexSize * 2);

    for (size_t i = 0; i < pGeometry->pos.size(); ++i)
    {
        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) 
        {
            vertexData.push_back(pGeometry->pos[i].x);
            vertexData.push_back(pGeometry->pos[i].y);
            vertexData.push_back(pGeometry->pos[i].z);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
        {
            vertexData.push_back(pGeometry->color[i].x);
            vertexData.push_back(pGeometry->color[i].y);
            vertexData.push_back(pGeometry->color[i].z);
            vertexData.push_back(pGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) 
        {
            vertexData.push_back(pGeometry->uv[i].x);
            vertexData.push_back(pGeometry->uv[i].y);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) 
        {
            vertexData.push_back(pGeometry->normals[i].x);
            vertexData.push_back(pGeometry->normals[i].y);
            vertexData.push_back(pGeometry->normals[i].z);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(pGeometry->indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);

    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), pGeometry->indices.data(), ibByteSize);
    
    m_pBufferGeometryData->VertexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), vertexData.data(), vbByteSize, &m_pBufferGeometryData->VertexBufferUploader);
    m_pBufferGeometryData->IndexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), pGeometry->indices.data(), ibByteSize, &m_pBufferGeometryData->IndexBufferUploader);

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pGeometry->indices.size());
}


void GCMesh::AddGeometry(GCGeometry* pGeometry, DirectX::XMFLOAT3 position)
{
    m_geoAmount += 1;

    std::vector<float> vertexData;
    size_t vertexSize = 0;
    
    // Manage offset bitize for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) vertexSize += 3;
    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) vertexSize += 2;
    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) vertexSize += 3;
    
    vertexData.reserve(vertexData.size() + pGeometry->pos.size() * vertexSize * 2);
    
    for (size_t i = 0; i < pGeometry->pos.size(); ++i)
    {
        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION))
        {
            vertexData.push_back(pGeometry->pos[i].x + position.x);
            vertexData.push_back(pGeometry->pos[i].y + position.y);
            vertexData.push_back(pGeometry->pos[i].z + position.z);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
        {
            vertexData.push_back(pGeometry->color[i].x);
            vertexData.push_back(pGeometry->color[i].y);
            vertexData.push_back(pGeometry->color[i].z);
            vertexData.push_back(pGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_UV))
        {
            vertexData.push_back(pGeometry->uv[i].x);
            vertexData.push_back(pGeometry->uv[i].y);
        }
        if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL))
        {
            vertexData.push_back(pGeometry->normals[i].x);
            vertexData.push_back(pGeometry->normals[i].y);
            vertexData.push_back(pGeometry->normals[i].z);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(pGeometry->indices.size() * sizeof(std::uint16_t));

    BYTE* mappedData = nullptr;
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;

    HRESULT hr = m_pBufferGeometryData->VertexBufferUploader->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));
    
    memcpy(&mappedData[vertexSize * m_geoAmount * m_pBufferGeometryData->VertexBufferByteSize], vertexData.data(), sizeof(std::vector<float>*));

    m_pBufferGeometryData->VertexBufferGPU = m_pBufferGeometryData->VertexBufferUploader;

    mappedData = nullptr;

    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexBufferUploader->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));

    memcpy(&mappedData[pGeometry->indices.size() * m_geoAmount * m_pBufferGeometryData->IndexBufferByteSize], pGeometry->indices.data(), sizeof(std::vector<uint16_t>*));

    m_pBufferGeometryData->IndexBufferGPU = m_pBufferGeometryData->IndexBufferUploader;

}

//bool GCMesh::UpdateGeometryData(GCGeometry* pNewGeometry, int& newFlagEnabledBits)
//{
//    if (!pNewGeometry || !m_pBufferGeometryData) {
//        // Log or handle the error
//        return false;
//    }
//
//    m_flagEnabledBits = newFlagEnabledBits;
//
//    // Calculate new vertex size based on updated flags
//    size_t newVertexSize = 0;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION)) newVertexSize += 3;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR)) newVertexSize += 4;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) newVertexSize += 2;
//    if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL)) newVertexSize += 3;
//
//    std::vector<float> newVertexData;
//    newVertexData.reserve(pNewGeometry->pos.size() * newVertexSize * 2);
//
//    for (size_t i = 0; i < pNewGeometry->pos.size(); ++i)
//    {
//        if (HAS_FLAG(m_flagEnabledBits, HAS_POSITION))
//        {
//            newVertexData.push_back(pNewGeometry->pos[i].x);
//            newVertexData.push_back(pNewGeometry->pos[i].y);
//            newVertexData.push_back(pNewGeometry->pos[i].z);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_COLOR))
//        {
//            newVertexData.push_back(pNewGeometry->color[i].x);
//            newVertexData.push_back(pNewGeometry->color[i].y);
//            newVertexData.push_back(pNewGeometry->color[i].z);
//            newVertexData.push_back(pNewGeometry->color[i].w);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_UV)) 
//        {
//            newVertexData.push_back(pNewGeometry->uv[i].x);
//            newVertexData.push_back(pNewGeometry->uv[i].y);
//        }
//        if (HAS_FLAG(m_flagEnabledBits, HAS_NORMAL))
//        {
//            newVertexData.push_back(pNewGeometry->normals[i].x);
//            newVertexData.push_back(pNewGeometry->normals[i].y);
//            newVertexData.push_back(pNewGeometry->normals[i].z);
//        }
//    }
//
//    const UINT newVbByteSize = static_cast<UINT>(newVertexData.size() * sizeof(float));
//    const UINT newIbByteSize = static_cast<UINT>(pNewGeometry->indices.size() * sizeof(uint16_t));
//
//    // Map the upload buffer and copy the vertex data
//    void* pVertexDataBegin = nullptr;
//    HRESULT hr = m_pBufferGeometryData->VertexBufferUploader->Map(0, nullptr, &pVertexDataBegin);
//    if (FAILED(hr)) {
//        // Log or handle the error
//        return false;
//    }
//    memcpy(pVertexDataBegin, newVertexData.data(), newVbByteSize);
//    m_pBufferGeometryData->VertexBufferUploader->Unmap(0, nullptr);
//
//    // Map the upload buffer and copy the index data
//    void* pIndexDataBegin = nullptr;
//    hr = m_pBufferGeometryData->IndexBufferUploader->Map(0, nullptr, &pIndexDataBegin);
//    if (FAILED(hr)) {
//        // Log or handle the error
//        return false;
//    }
//    memcpy(pIndexDataBegin, pNewGeometry->indices.data(), newIbByteSize);
//    m_pBufferGeometryData->IndexBufferUploader->Unmap(0, nullptr);
//
//    // Update the buffer size and data
//    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(newVertexSize * sizeof(float));
//    m_pBufferGeometryData->VertexBufferByteSize = newVbByteSize;
//    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
//    m_pBufferGeometryData->IndexBufferByteSize = newIbByteSize;
//    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pNewGeometry->indices.size());
//
//    return true;
//}
