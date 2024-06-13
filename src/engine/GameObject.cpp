#include "pch.h"
#include "GameObject.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Components.h"



GCGameObject::GCGameObject( GCScene* pScene )
{
    m_ID = s_nextID++;
    m_pNode = nullptr;
    
    m_name = "";
    m_active = true;
    m_tag = "";
    m_layer = 0;
    
    m_pScene = pScene;
    m_pParent = nullptr;
}


// <summary> 
// Constructor for GCGameObject class. 
// This constructor initializes a new game object with the given parameters.
// It also increments the next ID for future game object.
// </summary>
// <param name="pScene"> A pointer to the scene where the game object will be created. </param>
// <param name="name"> A const char* representing the name of the game object. </param>
// <param name="pParent"> A pointer to the parent game object. </param>
// <param name="active"> A bool indicating whether the game object is active or not. </param>
// <param name="tag"> A const char* representing the tag of the game object. </param>
// <param name="layer"> An int representing the layer of the game object. </param>
GCGameObject::GCGameObject( GCScene* pScene, const char* name, GCGameObject* pParent, bool active, const char* tag, int layer ) 
{
    m_ID = s_nextID++;
    m_pNode = nullptr;
    
    m_name = name;
    m_active = active;
    m_tag = tag;
    m_layer = layer;
    
    m_pScene = pScene;
    m_pParent = pParent;
}


// <summary>
// Updates all the components of the game object.
// This function iterates through the components list of the game object and updates each component, if the game object and the component are active.
// The components are updated based on their TYPE, which ranges from 1 to 7 as of right now.
// </summary>
void GCGameObject::Update()
{
    if ( m_active == false ) return;
    Component* component; 
    for (int i = 1 ; i <= 7 ; i++)
        if ( m_componentsList.Find(i, component) == true ) 
        {
            if ( component->IsActive() == false ) continue;
            component->Update();
        }
}


// <summary>
// Destroys the game object and removes it from the scene.
// It removes the game object from the scene's list of game objects and destroys all its components
// </summary>
void GCGameObject::Destroy()
{
    GCSceneManager::AddGameObjectToDeleteQueue( this );
}








// <summary>
// 
// </summary>
void GCGameObject::RemoveComponent( int type )
{
    Component* component;
    if ( m_componentsList.Find( type, component ) == true )
    {
        delete component;
        m_componentsList.Remove( type );
    }
}


// <summary>
// 
// </summary>
void GCGameObject::ClearComponents()
{
    for ( int i = 1; i <= 7; i++ ) RemoveComponent( i );
}








// <summary>
// Creates a new child game object with custom parameters.
// This function creates a new child game object using the CreateGameObject method of the scene.
// The new child game object is then added to the children list of the current game object.
// </summary>
// <param name="name"> A const char* representing the name of the new child game object. Default is "GameObject". </param>
// <param name="active"> A bool indicating whether the new child game object is active or not. Default is true. </param>
// <param name="tag"> A const char* representing the tag of the new child game object. Default is an empty string. </param>
// <param name="layer"> An int representing the layer of the new child game object. Default is 0. </param>
void GCGameObject::CreateChild( const char* name, bool active, const char* tag, int layer )
{
    AddChild( m_pScene->CreateGameObject( name, active, tag, layer ) );
}


// <summary>
// Adds a child game object to the children list.
// This function adds the given game object as a child of the current game object.
// It also sets the parent of the child game object to the current game object.
// </summary>
// <param name="pChild"> A pointer to the game object to be added as a child. </param>
void GCGameObject::AddChild( GCGameObject* pChild )
{
    m_childrenList.PushBack( pChild );
    pChild->m_pParent = this;
}


// <summary>
// Moves a child game object from one index to another in the children list.
// This function removes the child game object at the given childIndex and inserts it at the newIndex in the children list.
// </summary>
// <param  name="childIndex"> An unsigned int representing the index of the child game object to be moved. </param>
// <param  name="newIndex"> An unsigned int representing the new index where the child game object should be placed. </param>
void GCGameObject::MoveChild( unsigned int childIndex, unsigned int newIndex )
{
    GCGameObject* pChild = m_childrenList.Get( childIndex );
    m_childrenList.Remove( childIndex );
    m_childrenList.Insert( newIndex, pChild );
}


void GCGameObject::RemoveChild( GCGameObject* pChild )
{
    RemoveChild( m_childrenList.GetIndex( pChild ) );
}


void GCGameObject::RemoveChild( unsigned int childIndex )
{
    m_childrenList.Remove( childIndex );
}


// <summary>
// Deletes a child game object at the specified index.
// This function destroys the child game object at the given index and removes it from the children list.
// </summary>
// <pparamname=" childIndex"> An unsigned int representing the index of the child game object to be deleted </param>
void GCGameObject::DeleteChild( unsigned int childIndex )
{
    m_childrenList.Get( childIndex )->Destroy();
    m_childrenList.Remove( childIndex );
}


// <summary>
// 
// </summary>
void GCGameObject::ClearChildren()
{
    for ( int i = 0; i < m_childrenList.GetSize(); i++ )
        m_childrenList.Get(i)->Destroy();
    m_childrenList.Clear();
}








// <summary>
// Sets the parent of the game object.
// This function removes the current game object from its parent's children list and adds it to the new parent's children list.
// </summary>
// <param name="pParent"> A pointer to the new parent game object. </param> 
void GCGameObject::SetParent( GCGameObject* pParent )
{
    m_pParent->RemoveChild( this );
    pParent->AddChild( this );
}