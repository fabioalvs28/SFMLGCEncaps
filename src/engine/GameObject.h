#pragma once
#include "../core/framework.h"

class Component;
class GCScene;



class GCGameObject
{
friend class GCScene;

protected:
    GCGameObject( GCScene* pScene );
    ~GCGameObject() = default;
    
    void Update();
    void Render();

public:
    void Destroy();
    
    void SetParent( GCGameObject* pParent );
    void RemoveParent();
    GCGameObject* CreateChild();
    void AddChild( GCGameObject* pChild );
    void RemoveChild( GCGameObject* pChild );
    void DeleteChild( GCGameObject* pChild );
    void DeleteChildren();
    
    void SetActive( bool active );
    void SetName( const char* name );
    void SetTag( const char* tag );
    void SetLayer( int layer );
    
    unsigned int GetID() const;
    GCScene* GetScene() const;
    GCGameObject* GetParent() const;
    GCList<GCGameObject*> GetChildren() const;
    bool IsActive() const;
    const char* GetName() const;
    const char* GetTag() const;
    int GetLayer() const;
    
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();
    template<class T>
    void RemoveComponent();
    void ClearComponents();

protected:
    void RemoveComponent( int type );

public:
    GCTransform m_transform;

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
    const char* m_tag;
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
    component->SetGameObject( this );
    m_componentsList.Insert( T::TYPE, component );
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
    if ( m_componentsList.Find( T::TYPE, component ) == true )
        return (T*) component;
    return nullptr;
}

//////////////////////////////////////////////////////////
/// @brief Removes a Component from the GameObject.
/// 
/// @tparam T The class of the Component to be removed.
//////////////////////////////////////////////////////////
template<class T>
void GCGameObject::RemoveComponent()
{
    RemoveComponent( T::TYPE );
}