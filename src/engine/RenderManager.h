#pragma once
#include "Components.h"

class GCGraphics;
class GCGeometry;
class Window;

class GCRenderManager
{
    friend class GCGameObject;
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
};