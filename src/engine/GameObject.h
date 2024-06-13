#pragma once
#include "../Core/framework.h"

class Component;
class GCScene;



class GCGameObject
{
friend class GCScene;
    
public:
    void Destroy();
    
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();
    template<class T>
    void RemoveComponent();
    void ClearComponents();
    
    void CreateChild( const char* name = "GameObject", bool active = true, const char* tag = "", int layer = 0 );
    void AddChild( GCGameObject* pChild );
    GCGameObject* GetChild( unsigned int childIndex ) { return m_childrenList.Get( childIndex ); }
    GCVector<GCGameObject*> GetChildren() { return m_childrenList; }
    void MoveChild( unsigned int childIndex, unsigned int newChildIndex );
    void RemoveChild( GCGameObject* pChild );
    void RemoveChild( unsigned int childIndex );
    void DeleteChild( unsigned int childIndex );
    void ClearChildren();
    
    void SetName( const char* name ) { m_name = name; }
    void SetActive( bool active ) { m_active = active; }
    void SetTag( const char* tag ) { m_tag = tag; }
    void SetLayer( int layer ) { m_layer = layer; }
    void SetScene( GCScene* pScene ) { m_pScene = pScene; }
    void SetParent( GCGameObject* pParent );
    
    int GetID() const { return m_ID; }
    const char* GetName() const { return m_name; }
    bool IsActive() const { return m_active; }
    const char* GetTag() const { return m_tag; }
    int GetLayer() const { return m_layer; }
    GCScene* GetScene() const { return m_pScene; }
    GCGameObject* GetParent() const { return m_pParent; }

private:
    GCGameObject( GCScene* pScene );
    GCGameObject( GCScene* pScene, const char* name, GCGameObject* pParent, bool active, const char* tag, int layer );
    ~GCGameObject();
    
    void Update();
    
    void RemoveComponent( int type );
    
    void SetNode( GCListNode<GCGameObject*>* pNode ) { m_pNode = pNode; }
    GCListNode<GCGameObject*>* GetNode() const { return m_pNode; }

protected:
    static inline int s_nextID = 0;
    int m_ID;
    GCListNode<GCGameObject*>* m_pNode;
    
    GCScene* m_pScene;
    GCGameObject* m_pParent;
    GCVector<GCGameObject*> m_childrenList;
    
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
    Component* component;
    if ( m_componentsList.Find( T::TYPE, component ) == true )
        return (T*) component;
    return nullptr;
}

template<class T>
void GCGameObject::RemoveComponent()
{
    RemoveComponent( T::TYPE );
}