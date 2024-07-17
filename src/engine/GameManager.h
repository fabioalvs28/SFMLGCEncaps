#pragma once
#include "PhysicManager.h"
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
    GCPhysicManager m_pPhysicManager;
    GCUpdateManager m_pUpdateManager;
    GCEventManager m_pEventManager;
    GCSceneManager m_pSceneManager;
};
