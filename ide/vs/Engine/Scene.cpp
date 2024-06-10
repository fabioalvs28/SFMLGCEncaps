#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"

void GCScene::Destroy()
{
	GCSceneManager::DestroyScene( this );
}

void GCScene::Load()
{
	GCSceneManager::LoadScene( this );
}

void GCScene::Unload()
{
	GCSceneManager::UnloadScene( this );
}


// void GCScene::LoadScene()
// {
// 	m_loadedSceneList.PushBack( this );
// 	m_loadedNode = m_loadedSceneList.GetLastNode(); 
// }

// void GCScene::UnloadScene()
// {
// 	m_loadedNode->Remove();
// }

// void GCScene::DestroyScene()
// {
// 	m_allSceneNode->DeepDestroy(); 
// }

// void GCScene::Update()
// {
// 	for (GCListNode<GCGameObject*>* gameObjectNode = m_gameObjectsList.GetFirstNode(); gameObjectNode != m_gameObjectsList.GetLastNode(); gameObjectNode = gameObjectNode->GetNext())
// 		gameObjectNode->GetData()->Update();
// }

// GCGameObject* GCScene::CreateGameObject( const char* name = "GameObject", bool active = true )
// {
// 	GCGameObject* gameObject = new GCGameObject();
// 	gameObject->Init( name, active );
// 	m_gameObjectsList.PushBack( gameObject );
// 	gameObject->SetNode(m_gameObjectsList.GetLastNode());
// 	return gameObject;
// }

// GCScene::GCScene( GCLinkedList<GCScene*>& loadedSceneList )
// {
// 	m_loadedSceneList = loadedSceneList;
// 	m_isActive = false;
// }