#include "pch.h"
#include "Scene.h"

#include "Components.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GC.h"


////////////////////////////////////////////////////////
/// @brief Default constructor for the GCScene class.
////////////////////////////////////////////////////////
GCScene::GCScene()
{
	m_pNode = nullptr;
	m_pLoadedNode = nullptr;
	m_pChildNode = nullptr;
	
	m_pParent = nullptr;
	
	m_name = "GCScene";
	m_active = false;
	m_pMainCamera = nullptr;
}

////////////////////////////////////////////////////
/// @brief Creates a new Scene.
/// 
/// @return A pointer to the newly created Scene.
////////////////////////////////////////////////////
GCScene* GCScene::Create() { return GC::GetActiveSceneManager()->CreateScene(); }

//////////////////////////////
/// @brief Loads the Scene.
//////////////////////////////
void GCScene::Load() { GC::GetActiveSceneManager()->LoadScene( this ); }

////////////////////////////////
/// @brief Unloads the Scene.
////////////////////////////////
void GCScene::Unload() { GC::GetActiveSceneManager()->UnloadScene( this ); }

/////////////////////////////////////////////////////////////////////////////
/// @brief Adds the Scene to the "Deletion Queue".
/// 
/// @note The Scene will be deleted the next frame.
/// @note The Scene's children will also be added to the "Deletion Queue".
/////////////////////////////////////////////////////////////////////////////
void GCScene::Destroy()
{
	GC::GetActiveSceneManager()->DestroyScene( this );
	DestroyGameObjects(); //? Maybe there will be an issue with destroying every GameObjects including the childs who are already destroyed by their parents.
	DestroyChildren();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a GameObject and adds it to the "Creation queue".
/// 
/// @return A pointer to the newly created GameObject.
/// 
/// @note It will not update nor will it render until the creation is completed (the next frame).
////////////////////////////////////////////////////////////////////////////////////////////////////
GCGameObject* GCScene::CreateGameObject()
{
	GCGameObject* pGameObject = new GCGameObject( this );
	pGameObject->m_pSceneNode = m_gameObjectsList.PushBack( pGameObject );
	GC::GetActiveSceneManager()->AddGameObjectToCreateQueue( pGameObject );
	return pGameObject;
}

GCGameObject* GCScene::CreateGameObject( GCGameObject* pParent )
{
	GCGameObject* pGameObject = new GCGameObject( this );
	pGameObject->m_pParent = pParent;
	pGameObject->m_pChildNode = pParent->m_childrenList.PushBack( pGameObject );
	pGameObject->m_globalActive = pParent->IsActive();
	GC::GetActiveSceneManager()->AddGameObjectToCreateQueue( pGameObject );
	return pGameObject;
}

//////////////////////////////////////////////////////////////////
/// @brief Moves the GameObject to this Scene.
/// 
/// @param pGameObject A pointer to the GameObject to be moved.
//////////////////////////////////////////////////////////////////
void GCScene::MoveGameObjectToScene( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to move a nullptr pGameObject to an other Scene" );
	ASSERT( pGameObject->m_pScene != this, LOG_WARNING, "Trying to move a GameObject to the Scene it's already in" );
	pGameObject->m_pScene->RemoveGameObjectFromScene( pGameObject );
	pGameObject->m_pSceneNode = m_gameObjectsList.PushBack( pGameObject );
	pGameObject->m_pScene = this;
}

///////////////////////////////////////////////////////////////////////////////////
/// @brief Removes the GameObject from the Scene.
/// 
/// @param pGameObject A pointer to the GameObject to be removed from the Scene.
/// 
/// @note The GameObject is not deleted. It is simply removed from the Scene.
///////////////////////////////////////////////////////////////////////////////////
void GCScene::RemoveGameObjectFromScene( GCGameObject* pGameObject )
{
	ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to move a nullptr pGameObject to an other Scene" );
	ASSERT( pGameObject->m_pScene == this, LOG_WARNING, "Trying to remove a GameObject from a Scene that doesn't contain it" );
    pGameObject->m_pSceneNode->Delete();
	pGameObject->m_pSceneNode = nullptr;
	pGameObject->m_pScene = nullptr;
}

////////////////////////////////////////////////////////////////////////////
/// @brief Finds a GameObject in the Scene based on its name.
/// 
/// @param name A string value indicating the searched GameObject's name.
/// 
/// @return A pointer to the searched GameObject.
////////////////////////////////////////////////////////////////////////////
GCGameObject* GCScene::FindGameObjectByName( const char* name )
{
	GCGameObject* pGameObject;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode!= nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pGameObject = pGameObjectNode->GetData();
		if ( pGameObject->m_name == name )
			return pGameObject;
	}
	return nullptr;
}

////////////////////////////////////////////////////////////////////
/// @brief Finds a GameObject in the Scene based on its ID.
/// 
/// @param ID An integer indicating the searched GameObject's ID.
/// 
/// @return A pointer to the searched GameObject.
////////////////////////////////////////////////////////////////////
GCGameObject* GCScene::FindGameObjectByID( int ID )
{
	GCGameObject* pGameObject;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pGameObject = pGameObjectNode->GetData();
		if ( pGameObject->m_ID == ID )
            return pGameObject;
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Adds every GameObject in the Scene to the "Deletion Queue".
/// 
/// @note The GameObjects will be deleted the next frame.
/// @note The GameObjects' children will also be added to the "Deletion Queue".
//////////////////////////////////////////////////////////////////////////////////
void GCScene::DestroyGameObjects()
{
	ASSERT( m_gameObjectsList.GetFirstNode() != nullptr, LOG_WARNING, "Trying to destroy GameObjects from an empty Scene" );
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		pGameObjectNode->GetData()->Destroy();
	m_gameObjectsList.Clear();
}



////////////////////////////////////////////////////
/// @brief Removes itself from it's parent Scene.
////////////////////////////////////////////////////
void GCScene::RemoveParent()
{
    ASSERT( m_pParent != nullptr, LOG_FATAL, "Trying to remove a Scene from a nullptr pParent" );
    m_pChildNode->Delete();
    m_pChildNode = nullptr;
    m_pParent = nullptr;
}

///////////////////////////////////////////////////////////////////////
/// @brief Creates a new Scene and adds it as a child to this Scene.
/// 
/// @return A pointer to the newly created Scene.
///////////////////////////////////////////////////////////////////////
GCScene* GCScene::CreateChild()
{
	GCScene* pChild = GC::GetActiveSceneManager()->CreateScene();
	AddChild( pChild );
	return pChild;
}

//////////////////////////////////////////////////////////////////
/// @brief Adds a child to this Scene.
/// 
/// @param pChild A pointer to the child Scene to be added.
/// 
/// @warning You can't add yourself as your child.
/// @warning You can't add the same child twice.
/// @warning You can't add one of your ancestors as your child.
/// 
/// @note It will remove the child previous parent.
//////////////////////////////////////////////////////////////////
void GCScene::AddChild( GCScene* pChild )
{
	ASSERT( pChild != nullptr, LOG_FATAL, "Trying to add a nullptr pChild to the Scene" );
	ASSERT( pChild != this, LOG_FATAL, "Trying to add a Scene as its own child" );
	ASSERT( pChild->m_pParent != this, LOG_FATAL, "Trying to add to a parent Scene a child that is already his" );
    for ( GCScene* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        ASSERT( pChild != pAncestor, LOG_FATAL, "Trying to add to a child Scene one of its ancestors (parents of parents...)" );
	
	if ( pChild->m_pParent != nullptr )
		pChild->RemoveParent();
	pChild->m_pChildNode = m_childrenList.PushBack( pChild );
	pChild->m_pParent = this;
}

////////////////////////////////////////////////
/// @brief Destroys every child of the Scene.
/// 
/// @note Does not destroy the parent Scene.
////////////////////////////////////////////////
void GCScene::DestroyChildren()
{
	ASSERT( m_childrenList.GetFirstNode() != nullptr, LOG_WARNING, "Trying to destroy the children of an empty Scene" );
    for ( GCListNode<GCScene*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Destroy();
    m_childrenList.Clear();
}



////////////////////////////////////////////////////////
/// @brief Sets a new parent to this Scene.
/// 
/// @param pParent A pointer to the new parent Scene.
////////////////////////////////////////////////////////
void GCScene::SetParent( GCScene* pParent )
{
	ASSERT( pParent != nullptr, LOG_FATAL, "Trying to set a nullptr pParent to the Scene" );
	pParent->AddChild( this );
}

///////////////////////////////////////////////
/// @return A pointer to the Scene's parent.
///////////////////////////////////////////////
GCScene* GCScene::GetParent() const { return m_pParent; }



Camera* GCScene::GetMainCamera()
{ return m_pMainCamera; }