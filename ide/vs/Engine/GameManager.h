#pragma once

class GameObject;

class SceneManager; 

class GameManager 
{

public:

    GameManager() {};
    virtual ~GameManager() {};
    
    void Init();
    void Update();
    
private:
    SceneManager* m_sceneManager; 
};