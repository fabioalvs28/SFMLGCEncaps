#pragma once
#include <map>
#include "../Core/framework.h"

class Component;



class GameObject
{

    friend class Scene; 
    friend class GameManager;
    
public:
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();

    void SetName( const char* name ) { m_name = name; };
    void SetActive( bool active ) { m_active = active; };
    void SetTag( const char* tag ) { m_tag = tag; };
    void SetNode( GCLinkedListNode<GameObject*>* pNode ) { m_node = pNode; };
    
    const char* GetName() const { return m_name; }
    bool IsActive() const { return m_active; }
    const char* GetTag() const { return m_tag; }

private:
    GameObject();
    ~GameObject();
    
    void Init( const char* name, bool active );
    void Update();
    void Destroy();

protected:
    static inline int s_nextID = 0;
    int m_ID;
    GCLinkedListNode<GameObject*>* m_node;
    const char* m_name;
    bool m_active;
    const char* m_tag;
    int m_layer;
    std::map<int, Component*> m_componentsList;

};

template<class T>
T* GameObject::AddComponent()
{
    T* component = new T();
    component.SetGameObject( this );
    m_componentsList[ T::GetType() ] = component;
    return component;
}

template<class T>
T* GameObject::GetComponent()
{
    return (T*) m_componentsList[ T::GetType() ];
}