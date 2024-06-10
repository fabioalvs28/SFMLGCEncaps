#pragma once
#include "../Core/framework.h"

class GameObject;

class GameManager 
{

public:
    GameManager() {};
    virtual ~GameManager() {};
    
    void Init();
    void Update();
    
    GameObject* CreateGameObject( const char* name/* = GameObject */, bool active/* = true */);

private:
    GCLinkedList<GameObject*> m_gameObjectsList;

};