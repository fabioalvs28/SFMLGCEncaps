#include "pch.h"
#include "GameManager.h"
#include "GameObject.h"



void GameManager::Update()
{
    for ( GCLinkedListNode<GameObject*>* gameObjectNode = m_gameObjectsList.GetFirstNode(); gameObjectNode != m_gameObjectsList.GetLastNode(); gameObjectNode = gameObjectNode->GetNext() )
        gameObjectNode->GetData()->Update();
}

GameObject* GameManager::CreateGameObject( const char* name = "GameObject", bool active = true )
{
    GameObject* gameObject = new GameObject();
    gameObject->Init( name, active );
    m_gameObjectsList.PushBack( gameObject );
    gameObject->SetNode( m_gameObjectsList.GetLastNode() );
    return gameObject;
}