#pragma once
#include "Components.h"
#include "../Render/pch.h"

using namespace DirectX;

class GCRenderManager
{
    friend class GCGameObject;
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
    XMFLOAT4X4 m_storedProjectionMatrix;
    XMFLOAT4X4 m_storedViewMatrix;

};