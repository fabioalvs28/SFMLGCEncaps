#include "pch.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "../core/framework.h"



void GCSceneManager::Update()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
			scene->Update();
	}
}

void GCSceneManager::NewDelete()
{
	GCGameObject* pGameObject;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToDeleteList.GetFirstNode(); pGameObjectNode != m_gameObjectsToDeleteList.GetLastNode(); pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pGameObject = pGameObjectNode->GetData();
		pGameObject->GetScene()->DestroyGameObject( pGameObject );
	}
	m_gameObjectsToDeleteList.Clear();

	for ( GCListNode<GCScene*>* pSceneNode = m_scenesToDeleteList.GetFirstNode(); pSceneNode != m_scenesToDeleteList.GetLastNode(); pSceneNode = pSceneNode->GetNext() )
		DestroyScene( pSceneNode->GetData() );
	m_scenesToDeleteList.Clear();
}

void GCSceneManager::Render()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
			scene->Render();
	}
}




GCScene* GCSceneManager::CreateScene( GCScene* pParent )
{
	GCScene* scene = new GCScene( pParent );
    m_scenesList.PushBack( scene );
	scene->SetNode( m_scenesList.GetLastNode() ); 
	return scene;
}

void GCSceneManager::LoadScene( GCScene* pScene )
{
	if ( pScene->GetLoadedNode() != nullptr ) return;
	m_loadedScenesList.PushBack( pScene );
	pScene->SetLoadedNode( m_loadedScenesList.GetLastNode() );
}

void GCSceneManager::UnloadScene( GCScene* pScene )
{
	GCListNode<GCScene*>* pLoadedNode = pScene->GetLoadedNode();
	if ( pLoadedNode == nullptr ) return;
	m_loadedScenesList.DeleteNode( pLoadedNode );
	pScene->RemoveLoadedNode();
}

void GCSceneManager::DestroyScene( GCScene* pScene )
{
	UnloadScene( pScene );
	m_scenesList.DeleteNode( pScene->GetNode() );
	pScene->ClearGameObjects();
	pScene->ClearChildren();
    delete pScene;
}


void GCSceneManager::AddGameObjectToDeleteQueue( GCGameObject* pGameObject )
{
	m_gameObjectsToDeleteList.PushBack( pGameObject );
}

void GCSceneManager::AddSceneToDeleteQueue( GCScene* pScene )
{
	m_scenesToDeleteList.PushBack( pScene );
}