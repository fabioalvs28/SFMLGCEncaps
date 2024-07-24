#include "pch.h"
#include "SceneManager.h"

#include "../core/framework.h"
#include "Scene.h"
#include "GameObject.h"

// todo RenderQueue



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates the GameObjects in the "Creation Queue" and Destroys the GameObjects in the "Deletion Queue".
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCSceneManager::Update()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToCreateList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		CreateGameObject( pGameObjectNode->GetData() );
	m_gameObjectsToCreateList.Clear();
	
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToDeleteList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		DestroyGameObject( pGameObjectNode->GetData() );
	m_gameObjectsToDeleteList.Clear();
}





void GCSceneManager::SetActiveScene( GCScene* pScene )
{
	ASSERT( pScene != nullptr, LOG_FATAL, "Trying to set a nullptr pScene as the active Scene" );
	if ( m_pActiveScene != nullptr )
		m_pActiveScene->m_active = false;
    m_pActiveScene = pScene;
    m_pActiveScene->m_active = true;
}

GCScene* GCSceneManager::GetActiveScene()
{ return m_pActiveScene; }



////////////////////////////////////////////////////
/// @brief Creates a new Scene.
/// 
/// @return A pointer to the newly created Scene.
////////////////////////////////////////////////////
GCScene* GCSceneManager::CreateScene()
{
	GCScene* pScene = new GCScene();
    pScene->m_pNode = m_scenesList.PushBack( pScene );
	if ( m_pActiveScene == nullptr )
		SetActiveScene( pScene );
	return pScene;
}

/////////////////////////////////////////////////////////
/// @brief Loads the Scene.
/// 
/// @param pScene A pointer to the Scene to be loaded.
/////////////////////////////////////////////////////////
void GCSceneManager::LoadScene( GCScene* pScene )
{
	ASSERT( pScene != nullptr, LOG_FATAL, "Trying to load a nullptr pScene" );
	if ( pScene->m_pLoadedNode != nullptr )
	{
		ASSERT( false, LOG_WARNING, "Trying to load an already loaded Scene" );
		return;
	}
	pScene->m_pLoadedNode = m_loadedScenesList.PushBack( pScene );
}

///////////////////////////////////////////////////////////
/// @brief Unloads the Scene.
/// 
/// @param pScene A pointer to the Scene to be unloaded.
///////////////////////////////////////////////////////////
void GCSceneManager::UnloadScene( GCScene* pScene )
{
	ASSERT( pScene != nullptr, LOG_FATAL, "Trying to unload a nullptr pScene" );
	ASSERT( pScene->m_pLoadedNode != nullptr, LOG_FATAL, "Trying to unload a Scene that isn't loaded" );
	pScene->m_pLoadedNode->Delete();
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
	ASSERT( pScene != nullptr, LOG_FATAL, "Trying to destroy a nullptr pScene" );
	UnloadScene( pScene );
	pScene->m_pNode->Delete();
	pScene->RemoveParent();
    delete pScene;
}



////////////////////////////////////////////////////////////////////
/// @brief Fully creates the GameObject.
/// 
/// @param pGameObject A pointer to the GameObject to be created.
////////////////////////////////////////////////////////////////////
void GCSceneManager::CreateGameObject( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to create a nullptr pGameObject (SceneManager)" );
	for ( auto it : pGameObject->m_componentsList )
		it.second->RegisterToManagers();
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
	ASSERT( pGameObject!= nullptr, LOG_FATAL, "Trying to destroy a nullptr pGameObject (SceneManager)" );
	if ( pGameObject->m_pSceneNode != nullptr )
		pGameObject->RemoveScene();
	if ( pGameObject->m_pParent != nullptr )
		pGameObject->RemoveParent();
	pGameObject->ClearComponents();
	delete pGameObject;
}



///////////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Deletion Queue".
/// 
/// @param pGameObject A pointer to the GameObject to be added to the queue.
///////////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddGameObjectToDeleteQueue( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to add a nullptr pGameObject to the Deletion Queue" );
	m_gameObjectsToDeleteList.PushBack( pGameObject );
}

///////////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Creation Queue".
/// 
/// @param pGameObject A pointer to the GameObject to be added to the queue.
///////////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddGameObjectToCreateQueue( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to add a nullptr pGameObject to the Creation Queue" );
	m_gameObjectsToCreateList.PushBack( pGameObject );
}