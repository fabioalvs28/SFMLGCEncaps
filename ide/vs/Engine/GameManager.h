#pragma once
#include <vector>
#include "GameObject.h"



class GameManager
{

public:
    static void Init();
    static GameManager* GetInstance() { return s_Instance; };
    
    static void Update();
    
    static GameObject* CreateGameObject( const char* name );

private:
    static GameManager* s_Instance;
    static std::vector<GameObject*> m_gameObjectsList;
    GameManager();
    ~GameManager();

};