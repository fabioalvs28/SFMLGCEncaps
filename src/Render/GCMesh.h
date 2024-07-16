#pragma once
struct GCPARTICLE
{
    GCGeometry* pParticleGeometry;
    DirectX::XMFLOAT3 initPos;
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 velocity;
    float lifetime;
    float age;
};

class GCMesh
{
public:
	GCMesh();
    ~GCMesh();

    void UploadGeometryData(GCGeometry* pGeometry, int& flagEnabledBits);

    bool Initialize(GCRender* pRender, GCGeometry* pGeometry, int& flagEnabledBits);

    inline GCMESHBUFFERDATA* GetBufferGeometryData() { return  m_pBufferGeometryData; }

    int GetFlagEnabledBits() const { return m_flagEnabledBits; }

    void AddGeometry(GCGeometry* pGeometry, DirectX::XMFLOAT3 position);

private:

    GCRender* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;
    int m_flagEnabledBits;
    int m_geoAmount;

    // #TODO Put this in shader or Other place
    ID3D12Resource* CreateDefaultBuffer(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        ID3D12Resource** uploadBuffer);

    // Particle System members
    //std::vector<GCPARTICLE> m_Particles;
    //ID3D12Resource* m_pParticleBufferGPU;
    //ID3D12Resource* m_pParticleBufferUploader;

};

