#pragma once

class GCSprite
{
friend class GCSpriteRenderer;
friend class GCAnimator;
friend class GCBoxCollider;
friend class GCCircleCollider;

public:
    GCSprite() { m_pGeometry = nullptr; m_pMesh = nullptr; m_pMaterial = nullptr; }
    GCSprite( std::string filename );
    ~GCSprite() {};

private:
    GCGeometry* m_pGeometry;
    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;
    
};