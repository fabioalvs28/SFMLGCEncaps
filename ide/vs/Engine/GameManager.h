#pragma once

class GameObject;
class GCSceneManager; 
class GCInputManager;

class GCGameManager 
{

public:
    GCGameManager() {};
    virtual ~GCGameManager() {};
    
    void Init();
    void Update();
    
private:
    GCSceneManager* m_pSceneManager;
    GCInputManager* m_pInputManager;

};