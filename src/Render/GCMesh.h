#pragma once
struct GCPARTICLE
{
    DirectX::XMFLOAT3 initPos;
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Velocity;
    float Lifetime;
    float Age;

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

    void InitializeParticleSystem(size_t maxParticles);
    void UpdateParticles(float deltaTime);
    void RenderParticles(GCGraphics* graphics, GCMaterial* material, DirectX::XMMATRIX viewProjMatrix);
private:


    GCRender* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;

    int m_flagEnabledBits;

    // #TODO Put this in shader or Other place
    ID3D12Resource* CreateDefaultBuffer(
        ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        ID3D12Resource** uploadBuffer);
    // Particle System members

    std::vector<GCPARTICLE> m_Particles;
    ID3D12Resource* m_pParticleBufferGPU;
    ID3D12Resource* m_pParticleBufferUploader;

};

