#pragma once

struct MeshBufferData
{
    // Give it a name so we can look it up by name.
    std::string Name;

    // System memory copies.  Use Blobs because the vertex/index format can be generic.
    // It is up to the client to cast appropriately.  
    ID3DBlob* VertexBufferCPU = nullptr;
    ID3DBlob* IndexBufferCPU = nullptr;

    ID3D12Resource* VertexBufferGPU = nullptr;
    ID3D12Resource* IndexBufferGPU = nullptr;

    ID3D12Resource* VertexBufferUploader = nullptr;
    ID3D12Resource* IndexBufferUploader = nullptr;

    // Data about the buffers.
    UINT VertexByteStride = 0;
    UINT VertexBufferByteSize = 0;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT IndexBufferByteSize = 0;


    UINT IndexCount = 0;

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView()const
    {
        D3D12_VERTEX_BUFFER_VIEW vbv;
        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.StrideInBytes = VertexByteStride;
        vbv.SizeInBytes = VertexBufferByteSize;

        return vbv;
    }

    D3D12_INDEX_BUFFER_VIEW IndexBufferView()const
    {
        D3D12_INDEX_BUFFER_VIEW ibv;
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes = IndexBufferByteSize;

        return ibv;
    }

    void DisposeUploaders()
    {
        VertexBufferUploader = nullptr;
        IndexBufferUploader = nullptr;
    }
};

class GCMesh
{
public:
	GCMesh();
    ~GCMesh();

    template<typename VertexStructType>
    void UploadGeometryData(GCGeometry* pGeometry);

    void UploadGeometryDataColor(GCGeometry* pGeometry);
    void UploadGeometryDataTexture(GCGeometry* pGeometry);

    void Initialize(GCRender* pRender) { m_pRender = pRender; }

    inline MeshBufferData* GetBufferGeometryData() { return  m_pBufferGeometryData; }

private:
    GCRender* m_pRender;


    MeshBufferData* m_pBufferGeometryData;

    // #TODO Put this in shader or Other place
    ID3D12Resource* CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ID3D12Resource* uploadBuffer);
};

template<typename VertexType>
void GCMesh::UploadGeometryData(GCGeometry* pGeometry) {
    std::vector<VertexType> vertices;

    vertices.resize(pGeometry->pos.size());

    if constexpr (std::is_same<VertexType, GCVERTEX>::value) 
    {
        for (size_t i = 0; i < pGeometry->pos.size(); ++i) 
        {
            vertices[i] = VertexType(pGeometry->pos[i], pGeometry->color[i]);
        }
    }
    else if constexpr (std::is_same<VertexType, GCVERTEXTEXTURE>::value) 
    {
        for (size_t i = 0; i < pGeometry->pos.size(); ++i) 
        {
            vertices[i] = VertexType(pGeometry->pos[i], pGeometry->texC[i]);
        }
    }
    const UINT vbByteSize = static_cast<UINT>(vertices.size() * sizeof(VertexType));
    const UINT ibByteSize = static_cast<UINT>(pGeometry->indices.size() * sizeof(std::uint16_t));

    m_pBufferGeometryData = new MeshBufferData();
    m_pBufferGeometryData->Name = "boxGeo";

    D3DCreateBlob(vbByteSize, &m_pBufferGeometryData->VertexBufferCPU);
    CopyMemory(m_pBufferGeometryData->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

    D3DCreateBlob(ibByteSize, &m_pBufferGeometryData->IndexBufferCPU);
    CopyMemory(m_pBufferGeometryData->IndexBufferCPU->GetBufferPointer(), pGeometry->indices.data(), ibByteSize);

    m_pBufferGeometryData->VertexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), vertices.data(), vbByteSize, m_pBufferGeometryData->VertexBufferUploader);
    m_pBufferGeometryData->IndexBufferGPU = CreateDefaultBuffer(m_pRender->Getmd3dDevice(), m_pRender->GetCommandList(), pGeometry->indices.data(), ibByteSize, m_pBufferGeometryData->IndexBufferUploader);

    m_pBufferGeometryData->VertexByteStride = sizeof(VertexType);
    m_pBufferGeometryData->VertexBufferByteSize = vbByteSize;
    m_pBufferGeometryData->IndexFormat = DXGI_FORMAT_R16_UINT;
    m_pBufferGeometryData->IndexBufferByteSize = ibByteSize;

    m_pBufferGeometryData->IndexCount = static_cast<UINT>(pGeometry->indices.size());
}