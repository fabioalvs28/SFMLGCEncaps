#include "pch.h"

//ID3D12Resource* GCMesh::CreateDefaultBuffer(ID3D12Device* device,ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ID3D12Resource* uploadBuffer)
//{
//    ID3D12Resource* defaultBuffer;
//
//    // Create the actual default buffer resource.
//    CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
//    CD3DX12_RESOURCE_DESC resDesc(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
//    device->CreateCommittedResource(
//        &heapProp,
//        D3D12_HEAP_FLAG_NONE,
//        &resDesc,
//        D3D12_RESOURCE_STATE_COMMON,
//        nullptr,
//        IID_PPV_ARGS(&defaultBuffer));
//
//    // In order to copy CPU memory data into our default buffer, we need to create
//    // an intermediate upload heap.
//
//    CD3DX12_HEAP_PROPERTIES heapPropUp(D3D12_HEAP_TYPE_UPLOAD);
//    CD3DX12_RESOURCE_DESC resDesc2(CD3DX12_RESOURCE_DESC::Buffer(byteSize));
//    device->CreateCommittedResource(
//        &heapPropUp,
//        D3D12_HEAP_FLAG_NONE,
//        &resDesc2,
//        D3D12_RESOURCE_STATE_GENERIC_READ,
//        nullptr,
//        IID_PPV_ARGS(&uploadBuffer));
//
//
//    // Describe the data we want to copy into the default buffer.
//    D3D12_SUBRESOURCE_DATA subResourceData = {};
//    subResourceData.pData = initData;
//    subResourceData.RowPitch = byteSize;
//    subResourceData.SlicePitch = subResourceData.RowPitch;
//
//    // Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
//    // will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
//    // the intermediate upload heap data will be copied to mBuffer.
//
//    CD3DX12_RESOURCE_BARRIER ResBarrier(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
//    cmdList->ResourceBarrier(1, &ResBarrier);
//
//    UpdateSubresources<1>(cmdList, defaultBuffer, uploadBuffer, 0, 0, 1, &subResourceData);
//
//    CD3DX12_RESOURCE_BARRIER ResBarrier2(CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
//    cmdList->ResourceBarrier(1, &ResBarrier2);
//
//
//    // Note: uploadBuffer has to be kept alive after the above function calls because
//    // the command list has not been executed yet that performs the actual copy.
//    // The caller can Release the uploadBuffer after it knows the copy has been executed.
//    return defaultBuffer;
//}

GCMesh::GCMesh()
    : m_pRender(nullptr),
    m_pBufferGeometryData(nullptr),
    m_pMeshGeometry(nullptr),
    m_flagEnabledBits(0),

    m_currentVertexUploadBufferSize(0),
    m_currentIndexUploadBufferSize(0),

    m_geoAmount(0)
{
}

GCMesh::~GCMesh()
{
    if (m_pBufferGeometryData)
    {
        m_pBufferGeometryData->VertexBufferCPU->Release();
        m_pBufferGeometryData->IndexBufferCPU->Release();

        m_pBufferGeometryData->VertexBufferGPU->Release();
        m_pBufferGeometryData->IndexBufferGPU->Release();

        m_pBufferGeometryData->VertexByteStride = 0;
        m_pBufferGeometryData->VertexBufferByteSize = 0;
        m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
        m_pBufferGeometryData->IndexBufferByteSize = 0;
        m_pBufferGeometryData->IndexCount = 0;

        DELETE(m_pBufferGeometryData);
    }
}

GC_GRAPHICS_ERROR GCMesh::Initialize(GCRenderContext* pRender, GCGeometry* pGeometry, int& flagEnabledBits)
{
    if (!CHECK_POINTERSNULL("Pointers pRender & pGeometry Valid", "Pointers pRender & pGeometry Not valid", pRender, pGeometry)) {
        return GCRENDER_ERROR_POINTER_NULL;
    }
    
    m_geoAmount = 1;
    m_geometryPositions.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

    m_pMeshGeometry = pGeometry;
    m_pRender = pRender;

    UploadGeometryData(flagEnabledBits);

    if (!CHECK_POINTERSNULL(
        "All mesh buffer data pointers are valid",
        "One or more mesh buffer data pointers are null",
        m_pBufferGeometryData->VertexBufferCPU,
        m_pBufferGeometryData->IndexBufferCPU,
        m_pBufferGeometryData->VertexBufferGPU,
        m_pBufferGeometryData->IndexBufferGPU
    )) 
    {
        return GCRENDER_ERROR_POINTER_NULL;
    };

    return GCRENDER_SUCCESS_OK;
}

void GCMesh::UploadGeometryData(int& flagEnabledBits) {
    m_flagEnabledBits = flagEnabledBits;

    std::vector<float> vertexData;
    size_t vertexSize = 0;

    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) vertexSize += 3;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) vertexSize += 2;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) vertexSize += 3;

    vertexData.reserve(m_pMeshGeometry->pos.size() * vertexSize);

    for (size_t i = 0; i < m_pMeshGeometry->pos.size(); ++i) {
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) {
            vertexData.push_back(m_pMeshGeometry->pos[i].x);
            vertexData.push_back(m_pMeshGeometry->pos[i].y);
            vertexData.push_back(m_pMeshGeometry->pos[i].z);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) {
            vertexData.push_back(m_pMeshGeometry->color[i].x);
            vertexData.push_back(m_pMeshGeometry->color[i].y);
            vertexData.push_back(m_pMeshGeometry->color[i].z);
            vertexData.push_back(m_pMeshGeometry->color[i].w);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) {
            vertexData.push_back(m_pMeshGeometry->uv[i].x);
            vertexData.push_back(m_pMeshGeometry->uv[i].y);
        }
        if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) {
            vertexData.push_back(m_pMeshGeometry->normals[i].x);
            vertexData.push_back(m_pMeshGeometry->normals[i].y);
            vertexData.push_back(m_pMeshGeometry->normals[i].z);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(m_pMeshGeometry->indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);
    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), m_pMeshGeometry->indices.data(), ibByteSize);

    auto vertexBuffer = std::make_unique<GCUploadBuffer<float>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(vertexData.size()));
    auto indexBuffer = std::make_unique<GCUploadBuffer<std::uint16_t>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(m_pMeshGeometry->indices.size()));
    
    m_currentIndexUploadBufferSize = m_pMeshGeometry->indices.size();
    m_currentVertexUploadBufferSize = vertexData.size();

    vertexBuffer->CopyData(0, vertexData.data(), vbByteSize);
    indexBuffer->CopyData(0, m_pMeshGeometry->indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = vertexBuffer->Resource();
    m_pBufferGeometryData->IndexBufferGPU = indexBuffer->Resource();
    m_pBufferGeometryData->VertexBufferUploader = vertexBuffer.release()->Resource();
    m_pBufferGeometryData->IndexBufferUploader = indexBuffer.release()->Resource();

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(m_pMeshGeometry->indices.size());
}

void GCMesh::AddGeometry(DirectX::XMFLOAT3 position)
{
    m_geometryPositions.push_back(position);
    m_geoAmount += 1;
}

void GCMesh::DeleteGeometryAt(int index)
{
    m_geometryPositions.erase(m_geometryPositions.begin() + index);
    m_geoAmount -= 1;
}

void GCMesh::EditGeometryPositionAt(int index, DirectX::XMFLOAT3 newPosition)
{
    m_geometryPositions[index] = newPosition;
}

void GCMesh::UpdateGeometryData(float deltaTime)
{
    std::vector<float> vertexData;
    size_t vertexSize = 0;

    // Manage offset bits size for reserve vector
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) vertexSize += 3;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) vertexSize += 4;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) vertexSize += 2;
    if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) vertexSize += 3;

    vertexData.reserve(m_pMeshGeometry->pos.size() * vertexSize * m_geoAmount);

    for (int instance = 0; instance < m_geoAmount; ++instance) {

        for (size_t i = 0; i < m_pMeshGeometry->pos.size(); ++i) {
            if (HAS_FLAG(m_flagEnabledBits, VERTEX_POSITION)) {
                vertexData.push_back(m_pMeshGeometry->pos[i].x + m_geometryPositions[instance].x);
                vertexData.push_back(m_pMeshGeometry->pos[i].y + m_geometryPositions[instance].y);
                vertexData.push_back(m_pMeshGeometry->pos[i].z + m_geometryPositions[instance].z);
            }
            if (HAS_FLAG(m_flagEnabledBits, VERTEX_COLOR)) {
                vertexData.push_back(m_pMeshGeometry->color[i].x);
                vertexData.push_back(m_pMeshGeometry->color[i].y);
                vertexData.push_back(m_pMeshGeometry->color[i].z);
                vertexData.push_back(m_pMeshGeometry->color[i].w);
            }
            if (HAS_FLAG(m_flagEnabledBits, VERTEX_UV)) {
                vertexData.push_back(m_pMeshGeometry->uv[i].x);
                vertexData.push_back(m_pMeshGeometry->uv[i].y);
            }
            if (HAS_FLAG(m_flagEnabledBits, VERTEX_NORMAL)) {
                vertexData.push_back(m_pMeshGeometry->normals[i].x);
                vertexData.push_back(m_pMeshGeometry->normals[i].y);
                vertexData.push_back(m_pMeshGeometry->normals[i].z);
            }
        }
    }

    // Duplicate the index data for 2 instances
    std::vector<std::uint16_t> indices;
    indices.reserve(m_pMeshGeometry->indices.size() * 20);
    for (int instance = 0; instance < m_geoAmount; ++instance) {
        std::uint16_t baseIndex = static_cast<std::uint16_t>(instance * m_pMeshGeometry->pos.size());
        for (auto index : m_pMeshGeometry->indices) {
            indices.push_back(index + baseIndex);
        }
    }

    const UINT vbByteSize = static_cast<UINT>(vertexData.size() * sizeof(float));
    const UINT ibByteSize = static_cast<UINT>(indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertexData.data(), vbByteSize);
    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

    auto vertexBuffer = std::make_unique<GCUploadBuffer<float>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(vertexData.size()));
    auto indexBuffer = std::make_unique<GCUploadBuffer<std::uint16_t>>(m_pRender->GetRenderResources()->Getmd3dDevice(), static_cast<UINT>(indices.size()));

    vertexBuffer->CopyData(0, vertexData.data(), vbByteSize);
    indexBuffer->CopyData(0, indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = vertexBuffer->Resource();
    m_pBufferGeometryData->IndexBufferGPU = indexBuffer->Resource();
    m_pBufferGeometryData->VertexBufferUploader = vertexBuffer.release()->Resource();
    m_pBufferGeometryData->IndexBufferUploader = indexBuffer.release()->Resource();

    m_pBufferGeometryData->VertexByteStride = static_cast<UINT>(vertexSize * sizeof(float));
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(indices.size());
}
