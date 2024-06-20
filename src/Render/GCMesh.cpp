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
}

GCMesh::~GCMesh() 
{
}

void GCMesh::Initialize(GCRender* pRender, GCGeometry* pGeometry) {
    m_pRender = pRender;

    if (pGeometry->texC.size() == 0)
    {
        UploadGeometryDataColor(pGeometry);
    }
    else
    {
        UploadGeometryDataTexture(pGeometry);
    }
}

void GCMesh::UploadGeometryDataColor(GCGeometry* pGeometry) 
{

    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
    CHECK_POINTERSNULL(profiler, "Mesh geometry is NULL", "Mesh geometry has been initialized in Mesh successfully", pGeometry);

    UploadGeometryData<GCVERTEX>(pGeometry);

    CHECK_POINTERSNULL(profiler,
        "All mesh buffers have been successfully allocated",
        "One or more mesh buffers are not allocated",
        m_pBufferGeometryData->VertexBufferCPU,
        m_pBufferGeometryData->IndexBufferCPU,
        m_pBufferGeometryData->VertexBufferGPU,
        m_pBufferGeometryData->IndexBufferGPU,
        m_pBufferGeometryData->VertexBufferUploader,
        m_pBufferGeometryData->IndexBufferUploader
    );
}
void GCMesh::UploadGeometryDataTexture(GCGeometry* pGeometry) 
{
    GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();
    CHECK_POINTERSNULL(profiler, "Mesh geometry is NULL", "Mesh geometry has been initialized in Mesh successfully", pGeometry);

    UploadGeometryData<GCVERTEXTEXTURE>(pGeometry);

    CHECK_POINTERSNULL(profiler,
        "All mesh buffers have been successfully allocated",
        "One or more mesh buffers are not allocated",
        m_pBufferGeometryData->VertexBufferCPU,
        m_pBufferGeometryData->IndexBufferCPU,
        m_pBufferGeometryData->VertexBufferGPU,
        m_pBufferGeometryData->IndexBufferGPU,
        m_pBufferGeometryData->VertexBufferUploader,
        m_pBufferGeometryData->IndexBufferUploader
    );


}