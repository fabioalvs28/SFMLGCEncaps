#pragma once
#include "pch.h"
#include "../Render/pch.h"

using namespace DirectX;

class Animation;

class GCRenderManager
{
    friend class Component;
private:
    GCList<Component*> m_componentList;

public:
    GCRenderManager( Window* pWindow );
    ~GCRenderManager();

    void RegisterComponent(Component* pComponent);

    void CreateGeometry();

    void Render();

    void AddAnimation( Animation* animation , std::string animationName );
    Animation* GetAnimation( std::string animationName );

    GCGraphics* m_pGraphics; 
    GCGeometry* m_pPlane;
    GCGeometry* m_pCircle;


    //camera
    XMVECTOR m_cameraPosition ;
    XMVECTOR m_cameraTarget ;
    XMVECTOR m_cameraUp ;

protected:
    GCMap<std::string , Animation*> m_animationList;

};