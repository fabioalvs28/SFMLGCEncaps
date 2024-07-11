#pragma once
#include "PhysicManager.h"
#include "RenderManager.h"
#include "UpdateManager.h"
#include "EventManager.h"
#include "SceneManager.h"

class Component;
class Collider;
class GCGameObject;
class GCScene;
class GC;

class GCGameManager
{
    friend class SpriteRenderer;
    friend class Collider;
    friend class GCGameObject;
    friend class GCScene;
    friend class GC;
    friend class BoxCollider;
    friend class CircleCollider;

protected:
    GCGameManager() = default;
    virtual ~GCGameManager() = default;

public: void Init();
public: void Update();

protected:
//    GCKeyboardInputManager m_pInputManager;
    GCPhysicManager m_pPhysicManager;
    GCUpdateManager m_pUpdateManager;
    GCEventManager m_pEventManager;
    GCSceneManager m_pSceneManager;

public : 

    GCRenderManager m_pRenderManager;
};
