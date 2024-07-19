#include "pch.h"
#include "GameObject.h"

#include "GameObjectTransform.h"
#include "Components.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GC.h"

// TODO SetActive() also loops through the components



//////////////////////////////////////////////////////////////////////////////
/// @brief Default constructor for the GCGameObject class.
/// 
/// @param pScene A pointer to the Scene the GameObject is in.
/// 
/// @note The GameObject won't be fully created until the "Creation Queue".
//////////////////////////////////////////////////////////////////////////////
GCGameObject::GCGameObject( GCScene* pScene )
{
    ASSERT( pScene != nullptr, LOG_FATAL, "A nullptr pScene was given in the GameObject constructor" );
    m_ID = s_gameObjectsCount++;
    m_pSceneNode = nullptr;
    m_pChildNode = nullptr;
    
    m_pScene = pScene;
    m_pParent = nullptr;
    
    m_created = false;
    m_deleted = false;
    
    m_globalActive = true;
    m_selfActive = true;
    
    m_name = "GameObject";
    m_layer = 0;
    
    m_transform.m_pGameObject = this;
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
    
    if ( m_pParent != nullptr )
        m_pParent->AddChild( pGameObject );
    
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Duplicate()->SetParent( pGameObject );
    
    pGameObject->m_name = m_name;
    pGameObject->m_globalActive = m_globalActive;
    pGameObject->m_selfActive = m_selfActive;
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
    ASSERT( m_deleted == false, LOG_FATAL, "Trying to destroy a GameObject that is already going to be destroyed in the next frame" );
    m_deleted = true;
    DestroyChildren();
    GC::GetActiveSceneManager()->AddGameObjectToDeleteQueue( this );
}



/////////////////////////////////////////////////////////
/// @brief Removes itself from it's parent GameObject.
/////////////////////////////////////////////////////////
void GCGameObject::RemoveParent()
{
    ASSERT( m_pParent != nullptr, LOG_FATAL, "Trying to remove a child from a nullptr pParent" );
    m_pChildNode->Delete();
    m_pChildNode = nullptr;
    m_pParent = nullptr;
    m_transform.UpdateLocalMatrixFromWorld();
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief Creates a new GameObject and adds it as a child to this GameObject.
/// 
/// @return A pointer to the newly created GameObject.
/////////////////////////////////////////////////////////////////////////////////
GCGameObject* GCGameObject::CreateChild()
{
    GCGameObject* pChild = m_pScene->CreateGameObject();
    AddChild( pChild );
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
    ASSERT( pChild != nullptr, LOG_FATAL, "Trying to add a nullptr pChild to the GameObject" );
    ASSERT( pChild != this, LOG_FATAL, "Trying to add a GameObject as it's own child" );
    ASSERT( pChild->m_pParent != this, LOG_WARNING, "Trying to add to a parent GameObject a child that is already his" );
    for ( GCGameObject* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        ASSERT( pChild != pAncestor, LOG_FATAL, "Trying to add to a child GameObject one of its ancestors (parents of parents...)" );
    
    if ( pChild->m_pParent != nullptr )
        pChild->RemoveParent();
    
    pChild->m_pParent = this;
    pChild->m_pChildNode = m_childrenList.PushBack( pChild );
    pChild->m_transform.UpdateLocalMatrixFromWorld();
}

/////////////////////////////////////////////////////
/// @brief Destroys every child of the GameObject.
/// 
/// @note Does not destroy the parent GameObject.
/////////////////////////////////////////////////////
void GCGameObject::DestroyChildren()
{
    ASSERT( m_childrenList.GetFirstNode() != nullptr, LOG_WARNING, "Trying to destroy a GameObject's children even though he doesn't have any" );
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
    if ( m_tagsList.Find( tag ) == true )
    {
        ASSERT( false, LOG_WARNING, "Trying to add a tag to a GameObject that already has it" );
        return;
    }
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
    ASSERT( index != -1, LOG_FATAL, "Trying to remove a tag from a GameObject that doesn't have it" );
    RemoveTag( index );
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
void GCGameObject::SetScene( GCScene* pScene )
{
    ASSERT( pScene != nullptr, LOG_FATAL, "Trying to move a GameObject to a nullptr pScene" );
    pScene->MoveGameObjectToScene( this );
}

////////////////////////////////////////////////////
/// @brief Removes the GameObject from its Scene.
////////////////////////////////////////////////////
void GCGameObject::RemoveScene()
{
    ASSERT( m_pScene != nullptr, LOG_WARNING, "Trying to remove a GameObject from a nullptr pScene" );
    m_pScene->RemoveGameObjectFromScene( this );
}

/////////////////////////////////////////////////////////////
/// @brief Sets a new parent to this GameObject.
/// 
/// @param pParent A pointer to the new parent GameObject.
/////////////////////////////////////////////////////////////
void GCGameObject::SetParent( GCGameObject* pParent )
{
    ASSERT( pParent != nullptr, LOG_FATAL, "Trying to set a nullptr pParent as the parent of a GameObject" );
    pParent->AddChild( this );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the active state of this GameObject.
/// 
/// @param active A boolean value indicating whether the GameObject should be active or not.
/// 
/// @note When a GameObject is not active, it will not be updated or rendered.
/// @note Also sets its children's active state.
///////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetActive( const bool active )
{
    if ( IsActive() == true )
    {
        if ( active == false )
        {
            m_selfActive = false;
            for ( auto it : m_componentsList )
                it.second->SetGlobalActive( false );
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->SetGlobalActive( false );
        }
    }
    else
    {
        ASSERT( m_selfActive != active, LOG_WARNING, "Trying to SetActive() a GameObject with the same active state that is was already in" );
        m_selfActive = active;
        if ( IsActive() == true )
        {
            for ( auto it : m_componentsList )
                it.second->SetGlobalActive( true );
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->SetGlobalActive( true );
        }
    }
}

void GCGameObject::SetGlobalActive( const bool active )
{
    if ( IsActive() == true )
    {
        if ( active == false )
        {
            m_globalActive = false;
            for ( auto it : m_componentsList )
                it.second->SetGlobalActive( false );
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode!= nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->SetGlobalActive( false );
        }
    }
    else
    {
        m_globalActive = active;
        if ( IsActive() == true )
        {
            for ( auto it : m_componentsList )
                it.second->SetGlobalActive( true );
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode!= nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->SetGlobalActive( true );
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the name of this GameObject.
/// 
/// @param name A string value indicating the new name of the GameObject.
/// 
/// @note The name doesn't have to be unique within the Scene.
/// @note The name can be used for identification purposes (not recommanded).
////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetName( const char const* name ) { m_name = name; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the layer of this GameObject.
/// 
/// @param layer An integer indicating the new layer of the GameObject.
/// 
/// @note The layer determines the order in which GameObjects are rendered (higher layers are renderer on top).
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetLayer( const int layer ) { m_layer = layer; }



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
bool GCGameObject::IsActive() const { return m_globalActive && m_selfActive; }

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
    ASSERT( m_componentsList.Find( type, component ) == true, LOG_FATAL, "Trying to remove a component from a GameObject that doesn't have it" );
    delete component;
    m_componentsList.Remove( type );
}

////////////////////////////////////////////////////////////
/// @brief Removes every components from this GameObject.
////////////////////////////////////////////////////////////
void GCGameObject::ClearComponents()
{
    for ( auto it : m_componentsList )
        RemoveComponent( it.second->GetID() );
}