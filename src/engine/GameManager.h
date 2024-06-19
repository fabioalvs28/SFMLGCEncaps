#pragma once

class GCGameObject;
class GCScene;
class GCSceneManager; 
class GCInputManager;
class GC;

class GCGameManager 
{
friend class GCGameObject;
friend class GCScene;
friend class GC;

protected:
    GCGameManager() = default;
    virtual ~GCGameManager() = default;
    
    void Init();
    void Update();
    
protected:
    GCSceneManager m_pSceneManager;
    GCInputManager m_pInputManager;

};