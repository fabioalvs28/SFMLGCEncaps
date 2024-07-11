#pragma once
#include "Vector.h"
#include "Map.h"
#include "List.h"
#include "GameObjectTransform.h"

class Component;
class GCScene;
class GCSceneManager;



class GCGameObject
{
friend class GCScene;
friend class GCSceneManager;
friend class GCGameObjectTransform;

protected:
    ~GCGameObject() = default;

public:
    GCGameObject( GCScene* pScene );
    GCGameObject* Duplicate();
    void Destroy();
    
    void RemoveParent();
    GCGameObject* CreateChild();
    void AddChild( GCGameObject* pChild );
    void RemoveChild( GCGameObject* pChild );
    void DestroyChildren();
    
    void AddTag( const char* tag );
    void RemoveTag( const char* tag );
    void RemoveTag( int index );
    void RemoveTags();
    
    void SetScene( GCScene* pScene );
    void SetParent( GCGameObject* pParent );
    void SetActive( bool active );
    void SetName( const char* name );
    void SetLayer( int layer );
    
    unsigned int GetID() const;
    GCScene* GetScene() const;
    GCGameObject* GetParent() const;
    GCList<GCGameObject*>& GetChildren();
    bool IsActive() const;
    const char* GetName() const;
    const char* GetTag( int index ) const;
    int GetLayer() const;
    
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();
    template<class T>
    void RemoveComponent();
    void ClearComponents();

protected:
    void RemoveScene();
    
    void RemoveComponent( int type );

public:
    GCGameObjectTransform m_transform;

protected:
    static inline unsigned int s_gameObjectsCount = 0;
    unsigned int m_ID;
    GCListNode<GCGameObject*>* m_pSceneNode;
    GCListNode<GCGameObject*>* m_pChildNode;
    
    GCScene* m_pScene;
    GCGameObject* m_pParent;
    GCList<GCGameObject*> m_childrenList;
    
    bool m_created;
    bool m_active;
    const char* m_name;
    GCVector<const char*> m_tagsList;
    int m_layer;
    
    GCMap<int, Component*> m_componentsList;

};



////////////////////////////////////////////////////////
/// @brief Adds a new Component to the GameObject.
/// 
/// @tparam T The class of the Component to be added.
/// 
/// @return A pointer to the newly created Component.
////////////////////////////////////////////////////////
template<class T>
T* GCGameObject::AddComponent()
{
    if ( GetComponent<T>() != nullptr ) return nullptr;
    T* component = new T();
    component->m_pGameObject = this;
    m_componentsList.Insert( T::GetIDStatic(), component);
    return component;
}

///////////////////////////////////////////////////////////
/// @tparam T The class of the Component to be searched.
/// 
/// @return A pointer to the searched Component.
///////////////////////////////////////////////////////////
template<class T>
T* GCGameObject::GetComponent()
{
    Component* component;
    if ( m_componentsList.Find( T::GetIDStatic(), component ) == true )
        return (T*) component;
    return nullptr;
}

//////////////////////////////////////////////////////////
/// @brief Removes a Component from the GameObject.
/// 
/// @tparam T The class of the Component to be removed.
//////////////////////////////////////////////////////////
template<class T>
void GCGameObject::RemoveComponent() { RemoveComponent( T::TYPE ); }