#include "pch.h"
#include "GameManager.h"



void GameManager::Init()
{
    s_Instance = new GameManager();
}

void GameManager::Update()
{
    for ( int i = 0; i < m_gameObjectsList.size(); i++ )
        m_gameObjectsList[i]->Update();
}

GameObject* GameManager::CreateGameObject( const char* name = "GameObject", bool active = true )
{
    GameObject* gameObject = new GameObject();
    gameObject->Init( name, active );
    m_gameObjectsList.push_back( gameObject );
    return gameObject;
}