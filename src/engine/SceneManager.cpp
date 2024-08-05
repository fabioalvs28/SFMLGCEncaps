#include "pch.h"



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates the Components in the "Creation Queue" and Destroys the Components in the "Deletion Queue".
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCSceneManager::Update()
{
	for ( GCListNode<Component*>* pComponentNode = m_componentsToCreateList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
		CreateComponent( pComponentNode->GetData() );
	m_componentsToCreateList.Clear();
	
	for ( GCListNode<Component*>* pComponentNode = m_componentsToDeleteList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
		DestroyComponent( pComponentNode->GetData() );
	m_componentsToDeleteList.Clear();
	
    for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsToDeleteList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
	    DestroyGameObject( pGameObjectNode->GetData() );
	m_gameObjectsToDeleteList.Clear();
}



///////////////////////////////////////////////////////////////////////////
/// @brief Changes the active Scene to the given one.
/// 
/// @param pScene A pointer to the Scene to set as the new active Scene.
///////////////////////////////////////////////////////////////////////////
void GCSceneManager::SetActiveScene( GCScene* pScene )
{
	ASSERT( pScene != nullptr, LOG_FATAL, "Trying to set a nullptr pScene as the active Scene" );
	if ( m_pActiveScene != nullptr )
		m_pActiveScene->m_active = false;
    m_pActiveScene = pScene;
    m_pActiveScene->m_active = true;
}

/////////////////////////////////////////////
/// @return A pointer to the active scene.
/////////////////////////////////////////////
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
	GCGameObject* pMainCameraGameObject = pScene->CreateGameObject();
	pMainCameraGameObject->m_transform.SetPosition( GCVEC3( 0.0f, 0.0f, -1.0f ) );
	pScene->m_pMainCamera = pMainCameraGameObject->AddComponent<Camera>(); //! If you delete the GameObject that currently contains the MainCamera, it will make everything stop working and it will be the end of the world.
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



////////////////////////////////////////////////////////////////////////////
/// @brief Fully creates the Component by Registering it to the Managers.
/// 
/// @param pComponent A pointer to the Component to create.
////////////////////////////////////////////////////////////////////////////
void GCSceneManager::CreateComponent( Component* pComponent )
{
	ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to create a nullptr pComponent" );
	if ( pComponent->IsActive() == true )
	{
		pComponent->m_created = true;
		pComponent->RegisterToManagers();
	}
}

///////////////////////////////////////////////////////////////
/// @brief Fully destroys the Component.
/// 
/// @param pComponent A pointer to the Component to destroy.
///////////////////////////////////////////////////////////////
void GCSceneManager::DestroyComponent( Component* pComponent )
{
	ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to destroy a nullptr pComponent" );
	pComponent->Destroy();
	delete pComponent;
}



/////////////////////////////////////////////////////////////////
/// @brief Fully destroys the GameObject.
/// 
/// @param pGameObject A pointer to the GameObject to destroy.
/////////////////////////////////////////////////////////////////
void GCSceneManager::DestroyGameObject( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to destroy a nullptr pGameObject" );
	if ( pGameObject->m_pParent != nullptr )
		pGameObject->m_pChildNode->Delete();
	if ( pGameObject->m_pSceneNode != nullptr )
		pGameObject->m_pSceneNode->Delete();
	delete pGameObject;
}



////////////////////////////////////////////////////////////////////////
/// @brief Adds the Component to the "Creation Queue".
/// 
/// @param pComponent A pointer to the Component to add to the queue.
////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddToCreateQueue( Component* pComponent )
{
	ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to add a nullptr pComponent to the Creation Queue" );
	m_componentsToCreateList.PushBack( pComponent );
}

////////////////////////////////////////////////////////////////////////
/// @brief Adds the Component to the "Deletion Queue".
/// 
/// @param pComponent A pointer to the Component to add to the queue.
////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddToDeleteQueue( Component* pComponent )
{
	ASSERT( pComponent != nullptr, LOG_FATAL, "Trying to add a nullptr pComponent to the Deletion Queue" );
	m_componentsToDeleteList.PushBack( pComponent );
}



////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Deletion Queue".
/// 
/// @param pGameObject A pointer to the GameObject to add to the queue.
////////////////////////////////////////////////////////////////////////
void GCSceneManager::AddToDeleteQueue( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to add a nullptr pGameObject to the Deletion Queue" );
	m_gameObjectsToDeleteList.PushBack( pGameObject );
}