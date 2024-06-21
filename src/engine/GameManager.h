#pragma once
#include "SceneManager.h"
// #include "InputManager.h"

class GCGameObject;
class GCScene;
class GC;

class GCGameManager 
{
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
    // GCInputManager m_pInputManager;

};