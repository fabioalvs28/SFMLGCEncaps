#pragma once
#include "Components.h"
#include "../Render/pch.h"

using namespace DirectX;

class GCRenderManager
{
    friend class Component;
private:
    GCList<Component*> m_componentList;
public:
    GCRenderManager();
    ~GCRenderManager();

    void RegisterComponent(Component* pComponent);

    void CreateGeometry();

    void Render();

    GCGraphics* m_pGraphics; 
    GCGeometry* m_pPlane;
    GCGeometry* m_pCircle;


    //camera
    XMVECTOR m_cameraPosition ;
    XMVECTOR m_cameraTarget ;
    XMVECTOR m_cameraUp ;
    GCMATRIX m_storedProjectionMatrix;
    GCMATRIX m_storedViewMatrix;

};