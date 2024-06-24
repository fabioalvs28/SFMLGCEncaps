#pragma once

class GCMesh
{
public:
	GCMesh();
    ~GCMesh();


    void UploadGeometryData(GCGeometry* pGeometry);

    void Initialize(GCRender* pRender, GCGeometry* pGeometry);

    inline GCMESHBUFFERDATA* GetBufferGeometryData() { return  m_pBufferGeometryData; }

private:


    GCRender* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;

    // #TODO Put this in shader or Other place
    ID3D12Resource* CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize, ID3D12Resource* uploadBuffer);
};

