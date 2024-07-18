#pragma once
#include "PhysicManager.h"
#include "RenderManager.h"
#include "UpdateManager.h"
#include "EventManager.h"
#include "SceneManager.h"

class GCGameManager 
{
friend class GC;

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
