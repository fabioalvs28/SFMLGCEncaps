#pragma once

class GCSprite
{
friend class GCSpriteRenderer;
friend class GCAnimator;
friend class GCAnimation;
friend class GCBoxCollider;
friend class GCCircleCollider;

public:
    GCSprite() { m_pGeometry = nullptr; m_pMesh = nullptr; m_pMaterial = nullptr; }
    GCSprite( std::string projectname, std::string filename );
    ~GCSprite() {};

    GCGeometry* GetGeometry() { return m_pGeometry; };
private:
    GCGeometry* m_pGeometry;
    GCMesh* m_pMesh;
    GCMaterial* m_pMaterial;
    
};