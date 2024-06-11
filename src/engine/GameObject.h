#pragma once
#include "../Core/framework.h"

class Component;



class GCGameObject
{
friend class GCScene;
    
public:
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();
    template<class T>
    void RemoveComponent();

    void SetName( const char* name ) { m_name = name; };
    void SetActive( bool active ) { m_active = active; };
    void SetTag( const char* tag ) { m_tag = tag; };
    void SetNode( GCListNode<GCGameObject*>* pNode ) { m_pNode = pNode; };
    
    const char* GetName() const { return m_name; }
    bool IsActive() const { return m_active; }
    const char* GetTag() const { return m_tag; }
    int GetLayer() const { return m_layer; }
    GCListNode<GCGameObject*>* GetNode() const { return m_pNode; }

private:
    GCGameObject();
    GCGameObject( const char* name, bool active );
    GCGameObject( const char* name, bool active, const char* tag, int layer );
    ~GCGameObject();
    
    void Init( const char* name, bool active );
    void Update();

protected:
    static inline int s_nextID = 0;
    int m_ID;
    GCListNode<GCGameObject*>* m_pNode;
    const char* m_name;
    bool m_active;
    const char* m_tag;
    int m_layer;
    GCMap<int, Component*> m_componentsList;

};

template<class T>
T* GCGameObject::AddComponent()
{
    T* component = new T();
    component.SetGameObject( this );
    m_componentsList.Insert( T::TYPE, component );
    return component;
}

template<class T>
T* GCGameObject::GetComponent()
{
    T* component;
    if ( m_componentsList.Find( T::TYPE, component ) == true )
        return component;
}

template<class T>
void GCGameObject::RemoveComponent()
{
    T* component;
    if ( m_componentsList.Find( T::TYPE, component ) == true )
    {
        delete component;
        m_componentsList.Remove( T::TYPE );
    }
}