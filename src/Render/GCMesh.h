#pragma once
class GCMesh
{
public:
	GCMesh();
    ~GCMesh();

    GC_GRAPHICS_ERROR Initialize(GCRenderContext* pRender, GCGeometry* pGeometry, int& flagEnabledBits);

    void UploadGeometryData(GCGeometry* pGeometry, int& flagEnabledBits);

    inline GCMESHBUFFERDATA* GetBufferGeometryData() { return  m_pBufferGeometryData; }
    inline int GetFlagEnabledBits() const { return m_flagEnabledBits; }

private:
    GCRenderContext* m_pRender;
    GCMESHBUFFERDATA* m_pBufferGeometryData;

    int m_flagEnabledBits;
    int m_geoAmount;
};

