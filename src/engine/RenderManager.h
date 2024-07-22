#pragma once
#include "Components.h"
#include "../Render/pch.h"

using namespace DirectX;

class GCRenderManager
{
    friend class GCGameObject;
private:
    GCList<GCGameObject*> m_gameObjectList;
public:
    GCRenderManager();
    ~GCRenderManager();

    void RegisterGameObject(GCGameObject* pGameObject);

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