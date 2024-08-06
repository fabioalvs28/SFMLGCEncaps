#pragma once
#include "pch.h"
#include "../Render/pch.h"

using namespace DirectX;

class Animation;

class GCRenderManager
{
friend class GCComponent;

public:
    GCRenderManager( Window* pWindow );
    ~GCRenderManager();

    void RegisterComponent( GCComponent* pComponent );

    void CreateGeometry();

    void Render();

    void AddAnimation( Animation* pAnimation , std::string animationName );
    Animation* GetAnimation( std::string animationName );

    GCGraphics* m_pGraphics; 
    GCGeometry* m_pPlane;
    GCGeometry* m_pCircle;

private:
    GCList<GCComponent*> m_componentsList;
    GCMap<std::string , Animation*> m_animationsList;

};