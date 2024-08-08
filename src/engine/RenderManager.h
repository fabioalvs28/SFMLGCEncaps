#pragma once
#include "pch.h"

using namespace DirectX;

class GCRenderManager
{
friend class GCAnimation;
friend class GCSprite;
friend class GCComponent;
friend class GCSpriteRenderer;
friend class GCBoxCollider;
friend class GCAnimator;
friend class GCCamera;
friend class GCGameObject;
friend class GCGameManager;

private:
    GCRenderManager( Window* pWindow );
    ~GCRenderManager();

    void RegisterComponent( GCComponent* pComponent );
    
    void Render();

    void AddAnimation( GCAnimation* pAnimation , std::string animationName );
    GCAnimation* GetAnimation( std::string animationName );

private:
    GCGraphics* m_pGraphics;
    GCList<GCComponent*> m_componentsList;
    GCMap<std::string , GCAnimation*> m_animationsList;
    GCSprite* m_pSpriteCollider;

};