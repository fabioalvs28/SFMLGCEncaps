#pragma once

class GCMesh
{
public:
	GCMesh();
    ~GCMesh();

    template<typename VertexStructType>
    void UploadGeometryData(GCGeometry* pGeometry);

    void UploadGeometryDataColor(GCGeometry* pGeometry);
    void UploadGeometryDataTexture(GCGeometry* pGeometry);

    void Initialize(GCRender* pRender, GCGeometry* pGeometry);

    inline GCMESHBUFFERDATA* GetBufferGeometryData() { return  m_pBufferGeometryData; }

private:
    GCRender* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;

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

    m_pBufferGeometryData = new GCMESHBUFFERDATA();

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