#pragma once
#include <list>

class GameObject;

class GameManager 
{

public:
    static void Init();
    static GameManager* GetInstance() { return s_Instance; };
    
    static void Update();
    
    static GameObject* CreateGameObject( const char* name/* = GameObject */, bool active /* = true */);

private:
    static GameManager* s_Instance;
    static std::list<GameObject*> m_gameObjectsList;
    GameManager() {};
    ~GameManager() {};

};