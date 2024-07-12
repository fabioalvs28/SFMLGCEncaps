#pragma once
#include "SceneManager.h"
#include "PhysicManager.h"
#include "EventManager.h"

class Component;
class Collider;
class GCGameObject;
class GCScene;
class GC;

class GCGameManager 
{
friend class Component;
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
    GCSceneManager m_pSceneManager;
    GCPhysicManager m_pPhysicManager;
    GCEventManager m_pEventManager;
};
