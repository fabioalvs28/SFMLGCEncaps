#pragma once
#include "PhysicManager.h"
#include "RenderManager.h"
#include "UpdateManager.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "RenderManager.h"

// TODO Script inside GameManager

template <typename T>
class GCListNode;

class GCGameManager
{
    friend class GC;

public:
    void Run() {};

protected:
    GCGameManager();
    ~GCGameManager() = default;

public: void Update();

      void SetActiveGameManager();

protected:
    GCListNode<GCGameManager*>* m_pNode;
    GCPhysicManager m_pPhysicManager;
    GCUpdateManager m_pUpdateManager;
    GCEventManager m_pEventManager;
    GCSceneManager m_pSceneManager;
    GCRenderManager m_pRenderManager;
    GCInputSystem m_pInputSystem;
};
