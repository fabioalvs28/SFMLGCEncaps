#pragma once

class GCMesh
{
public:
	GCMesh();
    ~GCMesh();

    GC_GRAPHICS_ERROR Initialize(GCRenderContext* pRender, GCGeometry* pGeometry, int& flagEnabledBits);

    void UpdateGeometryData(float deltaTime);

    inline GCMESHBUFFERDATA* GetBufferGeometryData() { return  m_pBufferGeometryData; }
    inline int GetFlagEnabledBits() const { return m_flagEnabledBits; }

    void AddGeometry(DirectX::XMFLOAT3 position);
    void DeleteGeometryAt(int index);
    void EditGeometryPositionAt(int index, DirectX::XMFLOAT3 newPosition);

private:
    void UploadGeometryData(int& flagEnabledBits);

    GCRenderContext* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;

    GCGeometry* m_pMeshGeometry;
    std::vector<DirectX::XMFLOAT3> m_geometryPositions;

    int m_flagEnabledBits;

    int m_currentVertexUploadBufferSize;
    int m_currentIndexUploadBufferSize;

    GCUploadBufferBase* m_vertexBuffer;
    GCUploadBufferBase* m_indexBuffer;

    ID3D12Resource* CreateDefaultBuffer(ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        ID3D12Resource* uploadBuffer);
};

