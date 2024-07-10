#include "pch.h"
#include "GameObject.h"

#include "GameObjectTransform.h"
#include "Components.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GC.h"

// todo DESTROY( GameObject*& pGameObject ) -> also does pGameObject = nullptr



//////////////////////////////////////////////////////////////////////////////
/// @brief Default constructor for the GCGameObject class.
/// 
/// @param pScene A pointer to the Scene the GameObject is in.
/// 
/// @note The GameObject won't be fully created until the "Creation Queue".
//////////////////////////////////////////////////////////////////////////////
GCGameObject::GCGameObject( GCScene* pScene )
{
    m_ID = s_gameObjectsCount++;
    m_pSceneNode = nullptr;
    m_pChildNode = nullptr;
    
    m_pScene = pScene;
    m_pParent = nullptr;
    
    m_created = false;
    m_active = true;
    m_name = "GameObject";
    m_layer = 0;
}

////////////////////////////////////////////////////////////////////////////
/// @brief Updates the GameObject and its Components.
/// 
/// @note The GameObject won't update if not active or not fully created.
////////////////////////////////////////////////////////////////////////////
void GCGameObject::Update()
{
    if ( m_active == false || m_created == false ) return;
    for ( auto it : m_componentsList )
        if ( it.second->m_active == true )
            it.second->Update();
}

////////////////////////////////////////////////////////////////////////////
/// @brief Renders the GameObject's Components related to rendering.
/// 
/// @note The GameObject won't render if not active or not fully created.
////////////////////////////////////////////////////////////////////////////
void GCGameObject::Render()
{
    if ( m_active == false || m_created == false ) return;
    for ( auto it : m_componentsList )
        if ( it.second->m_active == true )
            it.second->Render();
}

/////////////////////////////////////////////////////////
/// @brief Duplicates the GameObject.
/// 
/// @return A pointer to the newly created GameObject.
/// 
/// @note The children are also duplicated.
/////////////////////////////////////////////////////////
GCGameObject* GCGameObject::Duplicate()
{
    GCGameObject* pGameObject = m_pScene->CreateGameObject();
    m_pParent->AddChild( pGameObject );
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Duplicate()->SetParent( pGameObject );
    pGameObject->m_name = m_name;
    pGameObject->m_active = m_active;
    pGameObject->m_tagsList = m_tagsList;
    pGameObject->m_layer = m_layer;
    pGameObject->m_componentsList = m_componentsList;
    return pGameObject;
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Adds the GameObject to the "Deletion Queue".
/// 
/// @note The GameObject will be deleted the next frame.
/// @note The GameObject's children will also be added to the "Deletion Queue".
//////////////////////////////////////////////////////////////////////////////////
void GCGameObject::Destroy()
{
    GC::m_pActiveGameManager.m_pSceneManager.AddGameObjectToDeleteQueue( this );
    DestroyChildren();
}



/////////////////////////////////////////////////////////
/// @brief Removes itself from it's parent GameObject.
/////////////////////////////////////////////////////////
void GCGameObject::RemoveParent()
{
    if ( m_pParent == nullptr ) return;
    m_pParent->RemoveChild( this );
    // todo Warn for the errors instead of simply returning nothing
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new GameObject and adds it as a child to this GameObject.
/// 
/// @return A pointer to the newly created GameObject.
/////////////////////////////////////////////////////////////////////////////////
GCGameObject* GCGameObject::CreateChild()
{
    GCGameObject* pChild = m_pScene->CreateGameObject();
    AddChild( pChild ); //! Not really efficient ( verifications could be skipped as the GameObject was just created )
    return pChild;
}

///////////////////////////////////////////////////////////////////
/// @brief Adds a child to this GameObject.
/// 
/// @param pChild A pointer to the child GameObject to be added.
/// 
/// @warning You can't add yourself as your child.
/// @warning You can't add the same child twice.
/// @warning You can't add one of your ancestors as your child.
/// 
/// @note It will remove the child previous parent.
///////////////////////////////////////////////////////////////////
void GCGameObject::AddChild( GCGameObject* pChild )
{
    if ( pChild == this ) return;
    if ( pChild->m_pParent == this ) return;
    for ( GCGameObject* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        if ( pChild == pAncestor ) return;
    
    pChild->RemoveParent();
    pChild->m_pChildNode = m_childrenList.PushBack( pChild );
    pChild->m_pParent = this;
    // todo Updating the transform so that the GameObject stays where it was before it was added
    // todo Assert for the errors instead of simply returning nothing
}

///////////////////////////////////////////////////////////////////////////////////////////
/// @brief Removes a child from this GameObject.
/// 
/// @param pChild A pointer to the child GameObject to be removed.
/// 
/// @warning You can't remove a GameObject that's not a direct child of this GameObject.
///////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::RemoveChild( GCGameObject* pChild )
{
    if ( pChild->m_pParent != this ) return;
    
    pChild->m_pChildNode->Delete();
    pChild->m_pChildNode = nullptr;
    pChild->m_pParent = nullptr;
    // todo Updating the transform so that the GameObject stays where it was before it was removed
    // todo Assert for the errors instead of simply returning nothing
}

/////////////////////////////////////////////////////
/// @brief Destroys every child of the GameObject.
/// 
/// @note Does not destroy the parent GameObject.
/////////////////////////////////////////////////////
void GCGameObject::DestroyChildren()
{
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Destroy();
    m_childrenList.Clear();
}



/////////////////////////////////////////////////////////////////////////////
/// @brief Adds a tag to the GameObject.
/// 
/// @param tag A string value indicating the tag to add to the GameObject.
/////////////////////////////////////////////////////////////////////////////
void GCGameObject::AddTag( const char* tag )
{
    if ( m_tagsList.Find( tag ) == true ) return;
    m_tagsList.PushBack( tag );
}

//////////////////////////////////////////////////////////////////////////////////
/// @brief Removes a tag from the GameObject.
/// 
/// @param tag A string value indicating the tag to remove from the GameObject.
//////////////////////////////////////////////////////////////////////////////////
void GCGameObject::RemoveTag( const char* tag )
{
    int index = m_tagsList.GetIndex( tag );
    if ( index == -1 ) return;
    RemoveTag( index );
    // todo Warn for the errors instead of simply returning nothing
}

/////////////////////////////////////////////////////////////////////////
/// @brief Removes a tag from the GameObject.
/// 
/// @param index An integer indicating the index of the tag to remove.
/////////////////////////////////////////////////////////////////////////
void GCGameObject::RemoveTag( int index ) { m_tagsList.Remove( index ); }

///////////////////////////////////////////////////
/// @brief Removes all tags from the GameObject.
///////////////////////////////////////////////////
void GCGameObject::RemoveTags() { m_tagsList.Clear(); }



////////////////////////////////////////////////////////////////////////////
/// @brief Moves the GameObject to a new Scene.
/// 
/// @param pScene A pointer to the Scene the GameObject will be moved to.
////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetScene( GCScene* pScene ) { pScene->MoveGameObjectToScene( this ); }

////////////////////////////////////////////////////
/// @brief Removes the GameObject from its Scene.
////////////////////////////////////////////////////
void GCGameObject::RemoveScene() { m_pScene->RemoveGameObjectFromScene( this ); }

/////////////////////////////////////////////////////////////
/// @brief Sets a new parent to this GameObject.
/// 
/// @param pParent A pointer to the new parent GameObject.
/////////////////////////////////////////////////////////////
void GCGameObject::SetParent( GCGameObject* pParent ) { pParent->AddChild( this ); }

///////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the active state of this GameObject.
/// 
/// @param active A boolean value indicating whether the GameObject should be active or not.
/// 
/// @note When a GameObject is not active, it will not be updated or rendered.
/// @note Also sets its children's active state.
///////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetActive( bool active )
{
    m_active = active;
    for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode->GetNext() )
        pGameObjectNode->GetData()->SetActive( active );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the name of this GameObject.
/// 
/// @param name A string value indicating the new name of the GameObject.
/// 
/// @note The name doesn't have to be unique within the Scene.
/// @note The name can be used for identification purposes (not recommanded).
////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetName( const char* name ) { m_name = name; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the layer of this GameObject.
/// 
/// @param layer An integer indicating the new layer of the GameObject.
/// 
/// @note The layer determines the order in which GameObjects are rendered (higher layers are renderer on top).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetLayer( int layer ) { m_layer = layer; }



//////////////////////////////////////////////////////////////////////
/// @return An integer indicating the unique ID of this GameObject.
//////////////////////////////////////////////////////////////////////
unsigned int GCGameObject::GetID() const { return m_ID; }

///////////////////////////////////////////////////////////
/// @return A pointer to the Scene the GameObject is in.
///////////////////////////////////////////////////////////
GCScene* GCGameObject::GetScene() const { return m_pScene; }

////////////////////////////////////////////////////
/// @return A pointer to the GameObject's parent.
////////////////////////////////////////////////////
GCGameObject* GCGameObject::GetParent() const { return m_pParent; }

//////////////////////////////////////////////////////////
/// @return A Linked List of the GameObject's children.
//////////////////////////////////////////////////////////
GCList<GCGameObject*>& GCGameObject::GetChildren() { return m_childrenList; }

/////////////////////////////////////////////////////////////////////////////
/// @return A boolean value indicating the active state of the GameObject.
/////////////////////////////////////////////////////////////////////////////
bool GCGameObject::IsActive() const { return m_active; }

////////////////////////////////////////////////////////////////////
/// @return A string value indicating the name of the GameObject.
////////////////////////////////////////////////////////////////////
const char* GCGameObject::GetName() const { return m_name; }

////////////////////////////////////////////////////////////////////////////
/// @return A string value indicating the searched tag of the GameObject.
////////////////////////////////////////////////////////////////////////////
const char* GCGameObject::GetTag( int index ) const { return m_tagsList[ index ]; }

/////////////////////////////////////////////////////////////////
/// @return An integer indicating the layer of the GameObject.
/////////////////////////////////////////////////////////////////
int GCGameObject::GetLayer() const { return m_layer; }



///////////////////////////////////////////////////////////
/// @brief Removes a component from this GameObject.
/// 
/// @param type The type of the component to be removed.
///////////////////////////////////////////////////////////
void GCGameObject::RemoveComponent( int type )
{
    Component* component;
    if ( m_componentsList.Find( type, component ) == true )
    {
        delete component;
        m_componentsList.Remove( type );
    }
}

////////////////////////////////////////////////////////////
/// @brief Removes every components from this GameObject.
////////////////////////////////////////////////////////////
void GCGameObject::ClearComponents()
{
    for ( auto it : m_componentsList )
        RemoveComponent( it.second->GetType() );
}