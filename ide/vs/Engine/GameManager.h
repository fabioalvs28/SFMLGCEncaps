#pragma once
#include <list>

class GameObject;

class GameManager 
{

public:
    static void Init();
    static GameManager* GetInstance() { return s_Instance; };
    
    static void Update();

private:
    static GameManager* s_Instance;
    static std::list<GameObject*> m_gameObjectsList;
    GameManager() {};
    ~GameManager() {};
    
    static GameObject* CreateGameObject( const char* name/* = GameObject */, bool active /* = true */);

};