#include "pch.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "../Core/framework.h"





Scene::Scene( GCLinkedList<Scene*>& loadedSceneList )
{
	m_loadedSceneList = loadedSceneList;
	m_isActive = false;
}

void SceneManager::CreateScene()
{
	Scene* scene = new Scene( m_loadedSceneList );
	m_sceneList.PushBack( scene );
	scene->m_allSceneNode = m_sceneList.GetLastNode();
}

void Scene::LoadScene()
{
	m_loadedSceneList.PushBack( this );
	m_loadedNode = m_loadedSceneList.GetLastNode(); 
}

void Scene::UnloadScene()
{
	m_loadedNode->Remove();
}

void Scene::DestroyScene()
{
	m_allSceneNode->DeepDestroy(); 
}

void SceneManager::Update() {

	for (GCLinkedListNode<Scene*>* sceneNode = m_loadedSceneList.GetFirstNode(); sceneNode != m_loadedSceneList.GetLastNode(); sceneNode = sceneNode->GetNext())
		sceneNode->GetData()->Update();
}

void SceneManager::RenderScene()
{
	for (GCLinkedListNode<Scene*>* sceneNode = m_sceneList.GetFirstNode(); sceneNode != m_sceneList.GetLastNode(); sceneNode = sceneNode->GetNext())
		if (sceneNode->GetData()->m_isActive == true)
		{
			// Render. 
		}
}



void Scene::Update()
{
	for (GCLinkedListNode<GameObject*>* gameObjectNode = m_gameObjectsList.GetFirstNode(); gameObjectNode != m_gameObjectsList.GetLastNode(); gameObjectNode = gameObjectNode->GetNext())
		gameObjectNode->GetData()->Update();
}


GameObject* Scene::CreateGameObject( const char* name = "GameObject", bool active = true )
{
	GameObject* gameObject = new GameObject();
	gameObject->Init( name, active );
	m_gameObjectsList.PushBack( gameObject );
	gameObject->SetNode(m_gameObjectsList.GetLastNode());
	return gameObject;
}


