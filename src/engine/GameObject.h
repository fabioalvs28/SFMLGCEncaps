#pragma once
#include "Define.h"
#include "Vector.h"
#include "Map.h"
#include "List.h"
#include "GameObjectTransform.h"
#include "Components.h"

class GCScene;



class GCGameObject
{
friend class GCGameObjectTransform;
friend class GCScene;
friend class GCSceneManager;
friend class GC;

protected:
    GCGameObject( GCScene* pScene );
    ~GCGameObject() = default;

public:
    GCGameObject* Duplicate();
    GCGameObject* Duplicate( GCGameObject* pParent );
    
    void RemoveParent();
    GCGameObject* CreateChild();
    void AddChild( GCGameObject* pChild );
    void DestroyChildren();
    
    void AddTag( const char* tag );
    void RemoveTag( const char* tag );
    void RemoveTags();
    
    void Activate();
    void Deactivate();
    
    void SetScene( GCScene* pScene );
    void SetParent( GCGameObject* pParent );
    void SetActive( const bool active );
    void SetName( const char const* name );
    void SetLayer( const int layer );
    
    unsigned int GetID() const;
    GCScene* GetScene() const;
    GCGameObject* GetParent() const;
    GCList<GCGameObject*>& GetChildren();
    bool IsActive() const;
    const char* GetName() const;
    const char* GetTag( int index ) const;
    int GetLayer() const;
    
    template <class T>
    T* AddComponent();
    template <class T>
    T* GetComponent();
    template <class T>
    void RemoveComponent();
    void ClearComponents();

protected:
    void Destroy();
    
    void RemoveTag( int index );
    
    void ActivateGlobal();
    void DeactivateGlobal();
    
    void RemoveScene();
    
    void RemoveComponent( int ID );
    template <class T>
    T* DuplicateComponent( GCGameObject* pGameObject );
    void DuplicateComponent( unsigned int ID, GCGameObject* pGameObject );

public:
    GCGameObjectTransform m_transform; // The transform that contains the GameObject's position, scale and rotation.

protected:
    static inline unsigned int s_gameObjectsCount = 0; // The amount of GameObjects created within the game.
    unsigned int m_ID; // The unique ID of the GameObject.
    GCListNode<GCGameObject*>* m_pSceneNode; // The Node of the GameObject in its Scene's GameObjects_List.
    GCListNode<GCGameObject*>* m_pChildNode; // The Node of the GameObject in its Parent's Children_List.
    
    GCScene* m_pScene; // The Scene where the GameObject located.
    GCGameObject* m_pParent; // The GameObject's Parent.
    GCList<GCGameObject*> m_childrenList; // The list of children the GameObject has.
    
    bool m_created; // A boolean value indicating if the GameObject is fully created.
    bool m_deleted; // A boolean value indicating that the GameObject is going to be deleted in the next frame.
    
    bool m_globalActive; // The global active state of the GameObject ( Usually its Parent active state ).
    bool m_selfActive; // The active state of the GameObject.
    
    const char* m_name; // The GameObject's name.
    GCVector<const char*> m_tagsList; // The list of tags the GameObject has.
    int m_layer; // The GameObject's layer.
    
    GCMap<int, Component*> m_componentsList; // The list of Components the GameObject has.

};



////////////////////////////////////////////////////////
/// @brief Adds a new Component to the GameObject.
/// 
/// @tparam T The class of the Component to add.
/// 
/// @return A pointer to the newly created Component.
////////////////////////////////////////////////////////
template <class T>
T* GCGameObject::AddComponent()
{
    ASSERT( GetComponent<T>() == nullptr, LOG_FATAL, "Trying to add a Component to a GameObject that already has it" );
    T* pComponent = new T();
    pComponent->m_pGameObject = this;
    bool gameObjectActive = IsActive();
    pComponent->m_globalActive = gameObjectActive;
    if ( gameObjectActive == true )
        pComponent->RegisterToManagers();
    m_componentsList.Insert( T::GetIDStatic(), pComponent );
    return pComponent;
}

/////////////////////////////////////////////////////
/// @tparam T The class of the searched Component.
/// 
/// @return A pointer to the searched Component.
/////////////////////////////////////////////////////
template <class T>
T* GCGameObject::GetComponent()
{
    Component* pComponent;
    if ( m_componentsList.Find( T::GetIDStatic(), pComponent ) == true )
        return (T*) pComponent;
    return nullptr;
}

//////////////////////////////////////////////////////
/// @brief Removes a Component from the GameObject.
/// 
/// @tparam T The class of the Component to remove.
//////////////////////////////////////////////////////
template <class T>
void GCGameObject::RemoveComponent() { RemoveComponent( T::GetIDStatic() ); }

template <class T>
T* GCGameObject::DuplicateComponent( GCGameObject* pGameObject )
{
    ASSERT( pGameObject != nullptr, LOG_FATAL, "Trying to duplicate a Component in a nullptr pGameObject" );
    T* pComponent = new T();
    *pComponent = *GetComponent<T>();
    pComponent->m_pGameObject = pGameObject;
    bool gameObjectActive = pGameObject->IsActive();
    pComponent->m_globalActive = gameObjectActive;
    if ( gameObjectActive == true )
        pComponent->RegisterToManagers();
    pGameObject->m_componentsList.Insert( T::GetIDStatic(), pComponent );
    return pComponent;
}

void GCGameObject::DuplicateComponent( unsigned int ID, GCGameObject* pGameObject ) //! Find a solution to automatically create the good one
{
    if ( ID == SpriteRenderer::GetIDStatic() )
        DuplicateComponent<SpriteRenderer>( pGameObject );
    
    else if ( ID == BoxCollider::GetIDStatic() )
        DuplicateComponent<BoxCollider>( pGameObject );
    
    else if ( ID == CircleCollider::GetIDStatic() )
        DuplicateComponent<CircleCollider>( pGameObject );
    
    else if ( ID == RigidBody::GetIDStatic() )
        DuplicateComponent<RigidBody>( pGameObject );
    
    else if ( ID == Animator::GetIDStatic() )
        DuplicateComponent<Animator>( pGameObject );
    
    else if ( ID == SoundMixer::GetIDStatic() )
        DuplicateComponent<SoundMixer>( pGameObject );
    
    else if ( ID == Camera::GetIDStatic() )
        DuplicateComponent<Camera>( pGameObject );
}