#include "pch.h"
#include "SceneManager.h"

#include "../core/framework.h"
#include "Scene.h"
#include "GameObject.h"

// todo RenderQueue



void GCSceneManager::Update()
{
	// todo Update
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
	// todo Render
}




GCScene* GCSceneManager::CreateScene()
{
	GCScene* pScene = new GCScene();
    m_scenesList.PushBack( pScene );
	pScene->m_pNode = m_scenesList.GetLastNode();
	return pScene;
}

void GCSceneManager::LoadScene( GCScene* pScene )
{
	if ( pScene->m_pLoadedNode != nullptr ) return;
	m_loadedScenesList.PushBack( pScene );
	pScene->m_pLoadedNode = m_loadedScenesList.GetLastNode();
}

void GCSceneManager::UnloadScene( GCScene* pScene )
{
	GCListNode<GCScene*>* pLoadedNode = pScene->m_pLoadedNode;
	if ( pLoadedNode == nullptr ) return;
	m_loadedScenesList.DeleteNode( pLoadedNode );
	pScene->m_pLoadedNode = nullptr;
}

void GCSceneManager::DestroyScene( GCScene* pScene )
{
	UnloadScene( pScene );
	m_scenesList.DeleteNode( pScene->m_pNode );
	pScene->DestroyGameObjects();
	pScene->DeleteChildren();
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