#include "pch.h"
#include "GameManager.h"
#include "GameObject.h"



void GameManager::Init()
{
    s_Instance = new GameManager();
}

void GameManager::Update()
{
    for ( auto it = m_gameObjectsList.begin(); it != m_gameObjectsList.end(); it++ )
        (*it)->Update();
}

GameObject* GameManager::CreateGameObject( const char* name = "GameObject", bool active = true )
{
    GameObject* gameObject = new GameObject();
    gameObject->Init( name, active );
    m_gameObjectsList.push_back( gameObject );
    return gameObject;
}