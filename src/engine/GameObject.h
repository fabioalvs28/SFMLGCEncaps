#pragma once
#include <map>
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
    void Destroy();

protected:
    static inline int s_nextID = 0;
    int m_ID;
    GCListNode<GCGameObject*>* m_pNode;
    const char* m_name;
    bool m_active;
    const char* m_tag;
    int m_layer;
    std::map<int, Component*> m_componentsList;

};

template<class T>
T* GCGameObject::AddComponent()
{
    T* component = new T();
    component.SetGameObject( this );
    m_componentsList[ T::TYPE ] = component;
    return component;
}

template<class T>
T* GCGameObject::GetComponent()
{
    return (T*) m_componentsList[ T::TYPE ];
}