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
}

////////////////////////////////////////////////////////////////
/// @brief Updates the Scene and all its GameObjects.
/// 
/// @note It will also update the parent Scene if it has one.
////////////////////////////////////////////////////////////////
void GCScene::Update()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		pGameObjectNode->GetData()->Update();
	if ( m_pParent != nullptr ) m_pParent->Update();
}

//////////////////////////////////////////////////////////////////////////////
/// @brief Renders all its GameObjects that can be rendered.
/// 
/// @note It will also render the parent Scene's GameObjects if it has one.
//////////////////////////////////////////////////////////////////////////////
void GCScene::Render()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		pGameObjectNode->GetData()->Render();
	if ( m_pParent != nullptr ) m_pParent->Render();
}

////////////////////////////////////////////////////
/// @brief Creates a new Scene.
/// 
/// @return A pointer to the newly created Scene.
////////////////////////////////////////////////////
GCScene* GCScene::Create() { return GC::m_pActiveGameManager.m_pSceneManager.CreateScene(); }

//////////////////////////////
/// @brief Loads the Scene.
//////////////////////////////
void GCScene::Load() { GC::m_pActiveGameManager.m_pSceneManager.LoadScene( this ); }

////////////////////////////////
/// @brief Unloads the Scene.
////////////////////////////////
void GCScene::Unload() { GC::m_pActiveGameManager.m_pSceneManager.UnloadScene( this ); }

/////////////////////////////////////////////////////////////////////////////
/// @brief Adds the Scene to the "Deletion Queue".
/// 
/// @note The Scene will be deleted the next frame.
/// @note The Scene's children will also be added to the "Deletion Queue".
/////////////////////////////////////////////////////////////////////////////
void GCScene::Destroy()
{
	GC::m_pActiveGameManager.m_pSceneManager.DestroyScene( this );
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
	GC::m_pActiveGameManager.m_pSceneManager.AddGameObjectToCreateQueue( pGameObject );
	return pGameObject;
}

//////////////////////////////////////////////////////////////////
/// @brief Moves the GameObject to this Scene.
/// 
/// @param pGameObject A pointer to the GameObject to be moved.
//////////////////////////////////////////////////////////////////
void GCScene::MoveGameObjectToScene( GCGameObject* pGameObject )
{
	if ( pGameObject->m_pScene == this ) return;
	pGameObject->m_pScene->RemoveGameObjectFromScene( pGameObject );
	m_gameObjectsList.PushBack( pGameObject );
	pGameObject->m_pSceneNode = m_gameObjectsList.GetLastNode();
	pGameObject->m_pScene = this;
	// todo Assert for the errors instead of returning nothing
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
	if ( pGameObject->m_pScene != this ) return;
    pGameObject->m_pSceneNode->Delete();
	pGameObject->m_pSceneNode = nullptr;
	pGameObject->m_pScene = nullptr;
	// todo Assert for the errors instead of returning nothing
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
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Adds every GameObject in the Scene to the "Deletion Queue".
/// 
/// @note The GameObjects will be deleted the next frame.
/// @note The GameObjects' children will also be added to the "Deletion Queue".
//////////////////////////////////////////////////////////////////////////////////
void GCScene::DestroyGameObjects()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode = pGameObjectNode->GetNext() )
		pGameObjectNode->GetData()->Destroy();
	m_gameObjectsList.Clear();
}



////////////////////////////////////////////////////
/// @brief Removes itself from it's parent Scene.
////////////////////////////////////////////////////
void GCScene::RemoveParent()
{
    if ( m_pParent == nullptr ) return;
    m_pParent->RemoveChild( this );
}

///////////////////////////////////////////////////////////////////////
/// @brief Creates a new Scene and adds it as a child to this Scene.
/// 
/// @return A pointer to the newly created Scene.
///////////////////////////////////////////////////////////////////////
GCScene* GCScene::CreateChild()
{
	GCScene* pChild = GC::m_pActiveGameManager.m_pSceneManager.CreateScene();
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
	if ( pChild == this ) return;
	if ( pChild->m_pParent == this ) return;
    for ( GCScene* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        if ( pChild == pAncestor ) return;
	
	pChild->RemoveParent();
	m_childrenList.PushBack( pChild );
	pChild->m_pChildNode = m_childrenList.GetLastNode();
	pChild->m_pParent = this;
    // todo Assert for the errors instead of simply returning nothing
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief Removes a child from this Scene.
/// 
/// @param pChild A pointer to the child Scene to be removed.
/// 
/// @warning You can't remove a Scene that's not a direct child of this Scene.
/////////////////////////////////////////////////////////////////////////////////
void GCScene::RemoveChild( GCScene* pChild )
{
    if ( pChild->m_pParent != this ) return;
    
    pChild->m_pChildNode->Delete();
    pChild->m_pChildNode = nullptr;
    pChild->m_pParent = nullptr;
    // todo Assert for the errors instead of simply returning nothing
}

////////////////////////////////////////////////
/// @brief Destroys every child of the Scene.
/// 
/// @note Does not destroy the parent Scene.
////////////////////////////////////////////////
void GCScene::DestroyChildren()
{
    for ( GCListNode<GCScene*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Destroy();
    m_childrenList.Clear();
}



////////////////////////////////////////////////////////
/// @brief Sets a new parent to this Scene.
/// 
/// @param pParent A pointer to the new parent Scene.
////////////////////////////////////////////////////////
void GCScene::SetParent( GCScene* pParent ) { pParent->AddChild( this ); }



///////////////////////////////////////////////
/// @return A pointer to the Scene's parent.
///////////////////////////////////////////////
GCScene* GCScene::GetParent() const { return m_pParent; }