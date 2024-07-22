#include "pch.h"
#include "GameObject.h"

#include "GameObjectTransform.h"
#include "Components.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GC.h"

//todo HasTag( const char* tag )



/////////////////////////////////////////////////////////////////////////////////
/// @brief Default constructor for the GameObject class.
/// 
/// @param pScene A pointer to the Scene where the GameObject will be created.
/// 
/// @note The GameObject will be fully created in the next frame. //? Is it necessary ?
/////////////////////////////////////////////////////////////////////////////////
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

GCGameObject* GCGameObject::Duplicate()
{
    if ( m_pParent != nullptr )
        this->Duplicate( m_pParent ); //info This does not duplicate the m_pParent !
    
    GCGameObject* pGameObject = m_pScene->CreateGameObject();
    
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Duplicate( pGameObject );
    
    pGameObject->m_transform = m_transform;
    pGameObject->m_globalActive = m_globalActive;
    pGameObject->m_selfActive = m_selfActive;
    pGameObject->m_name = m_name;
    pGameObject->m_tagsList = m_tagsList; // TODO Change this
    pGameObject->m_layer = m_layer;
    for ( auto it : m_componentsList )
        ;
    
    return pGameObject;
}

GCGameObject* GCGameObject::Duplicate( GCGameObject* pParent )
{
    ASSERT( pParent != nullptr, LOG_FATAL, "Trying to duplicate a GameObject in a nullptr pParent" );
    GCGameObject* pGameObject = pParent->CreateChild();
    
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Duplicate( pGameObject );
    
    pGameObject->m_transform = m_transform;
    pGameObject->m_globalActive = m_globalActive;
    pGameObject->m_selfActive = m_selfActive;
    pGameObject->m_name = m_name;
    pGameObject->m_tagsList = m_tagsList; // TODO Change this
    pGameObject->m_layer = m_layer;
    for ( auto it : m_componentsList )
        ;
    
    return pGameObject;
}

//////////////////////////////////////////////////////////////////////
/// @brief Destroys the GameObject.
/// 
/// @note The GameObject's children will also be destroyed.
/// @note The GameObject will be fully destroyed in the next frame. //? Is it necessary ?
//////////////////////////////////////////////////////////////////////
void GCGameObject::Destroy()
{
    ASSERT( m_deleted == false, LOG_FATAL, "Trying to destroy a GameObject that is already going to be destroyed in the next frame" ); //! There is will an issue with the Scene.Destroy() ! Maybe add a protected GameObject.DestroyNonRecursive()
    m_deleted = true;
    DestroyChildren();
    GC::GetActiveSceneManager()->AddGameObjectToDeleteQueue( this );
}




/////////////////////////////////////////////////////
/// @brief Removes the GameObject from its Parent.
/////////////////////////////////////////////////////
void GCGameObject::RemoveParent()
{
    ASSERT( m_pParent != nullptr, LOG_FATAL, "Trying to remove a child from a nullptr pParent" );
    m_pChildNode->Delete();
    m_pChildNode = nullptr;
    m_pParent = nullptr;
    m_transform.UpdateLocalMatrixFromWorld();
}

/////////////////////////////////////////////////////////////////////
/// @brief Creates a new GameObject as a child of this GameObject.
/// 
/// @return A pointer to the newly created child.
/////////////////////////////////////////////////////////////////////
GCGameObject* GCGameObject::CreateChild()
{ return m_pScene->CreateGameObject( this ); }

////////////////////////////////////////////////////////////////////////
/// @brief Adds an existing GameObject as a child of this GameObject.
/// 
/// @param pChild A pointer to the GameObject to add as a child.
/// 
/// @note If the GameObject already had a Parent, it will be removed.
////////////////////////////////////////////////////////////////////////
void GCGameObject::AddChild( GCGameObject* pChild )
{
    ASSERT( pChild != nullptr, LOG_FATAL, "Trying to add a nullptr pChild to the GameObject" );
    ASSERT( pChild != this, LOG_FATAL, "Trying to add a GameObject as it's own child" );
    ASSERT( pChild->m_pParent != this, LOG_WARNING, "Trying to add to a Parent GameObject a child that is already his" );
    for ( GCGameObject* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        ASSERT( pChild != pAncestor, LOG_FATAL, "Trying to add to a child GameObject one of its ancestors (Parents of Parents...)" );
    
    if ( pChild->m_pParent != nullptr )
        pChild->RemoveParent(); //info The could be optimized because it calls the transform's UpdateLocalMatrixFromWorld() 2 times
    
    pChild->m_pParent = this;
    pChild->m_pChildNode = m_childrenList.PushBack( pChild );
    pChild->m_globalActive = IsActive();
    pChild->m_transform.UpdateLocalMatrixFromWorld();
}

////////////////////////////////////////////////////////////////////
/// @brief Destroys every child of the GameObject.
/// 
/// @note The children will be fully destroyed in the next frame.
////////////////////////////////////////////////////////////////////
void GCGameObject::DestroyChildren()
{
    ASSERT( m_childrenList.GetFirstNode() != nullptr, LOG_WARNING, "Trying to destroy a GameObject's children even though he doesn't have any" );
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Destroy();
    m_childrenList.Clear();
}



/////////////////////////////////////////////////////////////////////////////////////
/// @brief Adds a tag to the GameObject.
/// 
/// @param tag A string value (const char*) indicating the name of the tag to add.
/////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::AddTag( const char* tag )
{
    ASSERT( m_tagsList.Find( tag ) == false, LOG_FATAL, "Trying to add a tag to a GameObject that already has it" );
    m_tagsList.PushBack( tag );
}

////////////////////////////////////////////////////////////////////////////////////////
/// @brief Removes a tag from the GameObject.
/// 
/// @param tag A string value (const char*) indicating the name of the tag to remove.
////////////////////////////////////////////////////////////////////////////////////////
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



void GCGameObject::Activate()
{
    if ( IsActive() == false )
    {
        m_selfActive = true;
        m_globalActive = true;
        for ( auto it : m_componentsList )
            it.second->ActivateGlobal();
        for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode->GetNext() )
            pGameObjectNode->GetData()->ActivateGlobal();
    }
}

void GCGameObject::Deactivate()
{
    if ( m_selfActive == true )
    {
        m_selfActive = false;
        if ( m_globalActive == true )
        {
            for ( auto it : m_componentsList )
                it.second->DeactivateGlobal();
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode != nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->DeactivateGlobal();
        }
    }
}

void GCGameObject::ActivateGlobal()
{
    if ( m_globalActive == false )
    {
        m_globalActive = true;
        if ( m_selfActive == true )
        {
            for ( auto it : m_componentsList )
                it.second->ActivateGlobal();
            for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode!= nullptr; pGameObjectNode->GetNext() )
                pGameObjectNode->GetData()->ActivateGlobal();
        }
    }
}

void GCGameObject::DeactivateGlobal()
{
    if ( m_globalActive == true )
    {
        m_globalActive = false;
        for ( auto it : m_componentsList )
            it.second->DeactivateGlobal();
        for ( GCListNode<GCGameObject*>* pGameObjectNode = m_childrenList.GetFirstNode(); pGameObjectNode!= nullptr; pGameObjectNode->GetNext() )
            pGameObjectNode->GetData()->DeactivateGlobal();
    }
}



///////////////////////////////////////////////////////////////////////////////
/// @brief Changes the Scene of the GameObject.
/// 
/// @param pScene A pointer to the Scene where the GameObject will be moved.
///////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetScene( GCScene* pScene )
{
    ASSERT( pScene != nullptr, LOG_FATAL, "Trying to move a GameObject to a nullptr pScene" );
    pScene->MoveGameObjectToScene( this );
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Removes the GameObject's Scene.
/// 
/// @warning The should only be used if you're planning to add it right away to another Scene.
/////////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::RemoveScene() //? Why this method ?
{
    ASSERT( m_pScene != nullptr, LOG_WARNING, "Trying to remove a GameObject from a nullptr pScene" );
    m_pScene->RemoveGameObjectFromScene( this );
}

////////////////////////////////////////////////////////////////////
/// @brief Changes the Parent of the GameObject.
/// 
/// @param pParent A pointer to the new Parent of the GameObject.
////////////////////////////////////////////////////////////////////
void GCGameObject::SetParent( GCGameObject* pParent ) //? Why this method ?
{
    ASSERT( pParent != nullptr, LOG_FATAL, "Trying to set a nullptr pParent as the Parent of a GameObject" );
    pParent->AddChild( this );
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the active state of the GameObject.
/// 
/// @param active A boolean value indicating whether the GameObject should be active or not.
/// 
/// @note The GameObject's children's active state will also be changed.
///////////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetActive( const bool active ) //todo If the active argument is true, it should also change the global active state.
{
    if ( active == true )
        Activate();
    else
        Deactivate();
}

//////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the name of the GameObject.
/// 
/// @param name A string value (const char*) indicating the new name of the GameObject.
/// 
/// @note The name doesn't have to be unique within the Scene.
//////////////////////////////////////////////////////////////////////////////////////////
void GCGameObject::SetName( const char const* name ) { m_name = name; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Sets the layer of the GameObject.
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

//////////////////////////////////////////////////////////////
/// @return A pointer to the Scene where the GameObject is.
//////////////////////////////////////////////////////////////
GCScene* GCGameObject::GetScene() const { return m_pScene; }

////////////////////////////////////////////////////
/// @return A pointer to the GameObject's Parent.
////////////////////////////////////////////////////
GCGameObject* GCGameObject::GetParent() const { return m_pParent; }

///////////////////////////////////////////////////
/// @return A List of the GameObject's children.
///////////////////////////////////////////////////
GCList<GCGameObject*>& GCGameObject::GetChildren() { return m_childrenList; }

//////////////////////////////////////////////////////////////////////////////////
/// @return A boolean value indicating whether the GameObject is active or not.
//////////////////////////////////////////////////////////////////////////////////
bool GCGameObject::IsActive() const { return m_globalActive && m_selfActive; }

//////////////////////////////////////////////////////////////////////////////////
/// @return A string value (const char*) indicating the name of the GameObject.
//////////////////////////////////////////////////////////////////////////////////
const char* GCGameObject::GetName() const { return m_name; }

//////////////////////////////////////////////////////////////////////////////////////////
/// @return A string value (const char*) indicating the searched tag of the GameObject.
//////////////////////////////////////////////////////////////////////////////////////////
const char* GCGameObject::GetTag( int index ) const { return m_tagsList[ index ]; } //? Why this method ?

/////////////////////////////////////////////////////////////////
/// @return An integer indicating the layer of the GameObject.
/////////////////////////////////////////////////////////////////
int GCGameObject::GetLayer() const { return m_layer; }



//////////////////////////////////////////////////////
/// @brief Removes a component from the GameObject.
/// 
/// @param ID The ID of the Component to remove.
//////////////////////////////////////////////////////
void GCGameObject::RemoveComponent( int ID )
{
    Component* pComponent;
    ASSERT( m_componentsList.Find( ID, pComponent ) == true, LOG_FATAL, "Trying to remove a Component from a GameObject that doesn't have it" ); //? The .Find() is necessary for the method to work but it's in an ASSERT ?
    delete pComponent;
    m_componentsList.Remove( ID );
}

///////////////////////////////////////////////////////////
/// @brief Removes every components from the GameObject.
///////////////////////////////////////////////////////////
void GCGameObject::ClearComponents()
{
    for ( auto it : m_componentsList )
        RemoveComponent( it.second->GetID() );
}