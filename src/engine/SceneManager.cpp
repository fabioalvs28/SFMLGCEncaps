#include "pch.h"
#include "SceneManager.h"

#include "../core/framework.h"
#include "Scene.h"
#include "GameObject.h"

// todo RenderQueue
// todo CreationQueue



///////////////////////////////////////
/// @brief Updates the active Scene.
///////////////////////////////////////
void GCSceneManager::Update() { m_pActiveScene->Update(); }

void GCSceneManager::NewDelete()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToCreateList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		CreateGameObject( pGameObjectNode->GetData() );
	m_gameObjectsToCreateList.Clear();
	
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToDeleteList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		DestroyGameObject( pGameObjectNode->GetData() );
	m_gameObjectsToDeleteList.Clear();
}

///////////////////////////////////////
/// @brief Renders the active Scene.
///////////////////////////////////////
void GCSceneManager::Render() { m_pActiveScene->Render(); }



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

////////////////////////////////////////////////////////////
/// @brief Fully destroys the Scene.
/// 
/// @param pScene A pointer to the Scene to be destroyed.
/// 
/// @note The Scene's GameObjects will also be destroyed.
////////////////////////////////////////////////////////////
void GCSceneManager::DestroyScene( GCScene* pScene )
{
	UnloadScene( pScene );
	m_scenesList.DeleteNode( pScene->m_pNode );
	pScene->RemoveParent();
    delete pScene;
}



void GCSceneManager::CreateGameObject( GCGameObject* pGameObject )
{
	GCScene* pScene = pGameObject->m_pScene;
	pScene->m_gameObjectsList.PushBack( pGameObject );
	pGameObject->m_pSceneNode = pScene->m_gameObjectsList.GetLastNode();
	pGameObject->m_created = true;
}

//////////////////////////////////////////////////////////////////////
/// @brief Fully destroys the GameObject.
/// 
/// @param pGameObject A pointer to the GameObject to be destroyed.
/// 
/// @note The GameObject's Components are also destroyed.
//////////////////////////////////////////////////////////////////////
void GCSceneManager::DestroyGameObject( GCGameObject* pGameObject )
{
	pGameObject->RemoveScene();
	pGameObject->RemoveParent();
	pGameObject->ClearComponents();
	delete pGameObject;
}



///////////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Deletion Queue".
/// 
/// @param pGameObject A pointer to the GameObject to be added to the queue.
///////////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddGameObjectToDeleteQueue( GCGameObject* pGameObject ) { m_gameObjectsToDeleteList.PushBack( pGameObject ); }

///////////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Creation Queue".
/// 
/// @param pGameObject A pointer to the GameObject to be added to the queue.
///////////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddGameObjectToDeleteQueue( GCGameObject* pGameObject ) { m_gameObjectsToCreateList.PushBack( pGameObject ); }