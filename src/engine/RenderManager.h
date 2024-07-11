#pragma once
#include "Components.h"

class GCGraphics;
class GCGeometry;
class Window;

class GCRenderManager
{
    friend class GCGameObject;

    struct GCTest : GCSHADERCB {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4 color;
    };

private:
    GCList<Component*> m_pComponentList;
public:
    GCRenderManager();
    ~GCRenderManager();

    void RegisterComponent(Component* pComponent);

    void CreateGeometry();

    void Render();

    GCGraphics* m_pGraphics; 
    GCGeometry* m_pPlane;
    GCGeometry* m_pCircle;

    DirectX::XMFLOAT4X4 m_storedProjectionMatrix;
    DirectX::XMFLOAT4X4 m_storedViewMatrix;

};