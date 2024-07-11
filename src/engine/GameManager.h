#pragma once
#include "SceneManager.h"
#include "PhysicManager.h"
#include "RenderManager.h"
#include "../../src/Render/Window.h"
#include "EventManager.h"

class Component;
class Collider;
class GCGameObject;
class GCScene;
class GC;
class SpriteRenderer;

class GCGameManager
{
    friend class SpriteRenderer;
    friend class Collider;
    friend class GCGameObject;
    friend class GCScene;
    friend class GC;

protected:
    GCGameManager() = default;
    virtual ~GCGameManager() = default;

public: void Init();
public: void Update();

protected:
//    GCKeyboardInputManager m_pInputManager;
//    GCPhysicManager m_pPhysicManager;
//    GCEventManager m_pEventManager;
    GCSceneManager m_pSceneManager;
    GCRenderManager m_pRenderManager;
    GCPhysicManager m_pPhysicManager;
    GCEventManager m_pEventManager;
};
