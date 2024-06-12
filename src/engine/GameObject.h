#pragma once
#include "../Core/framework.h"

class Component;
class GCScene;



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
    
    void SetNode( GCListNode<GCGameObject*>* pNode ) { m_pNode = pNode; }
    void SetName( const char* name ) { m_name = name; }
    void SetActive( bool active ) { m_active = active; }
    void SetTag( const char* tag ) { m_tag = tag; }
    void SetLayer( int layer ) { m_layer = layer; }
    void SetScene( GCScene* pScene ) { m_pScene = pScene; }
    void SetParent( GCGameObject* pParent );
    
    int GetID() const { return m_ID; }
    GCListNode<GCGameObject*>* GetNode() const { return m_pNode; }
    const char* GetName() const { return m_name; }
    bool IsActive() const { return m_active; }
    const char* GetTag() const { return m_tag; }
    int GetLayer() const { return m_layer; }
    GCScene* GetScene() const { return m_pScene; }
    GCGameObject* GetParent() const { return m_pParent; }
    
    void CreateChild();
    void CreateChild( const char* name /*= "GameObject"*/, bool active /*= true*/, const char* tag /*= ""*/, int layer /*= 0*/ );
    void AddChild( GCGameObject* pChild ); 
    void DeleteChild( unsigned int childIndex );
    void RemoveChild( GCGameObject* pChild );
    GCVector<GCGameObject*> GetChildren() { return m_childrenList; }
    GCGameObject* GetChild( unsigned int childIndex ) { return m_childrenList.Get( childIndex ); }
    void MoveChild( unsigned int childIndex, unsigned int newChildIndex );

private:
    GCGameObject( GCScene* pScene );
    GCGameObject( GCScene* pScene, const char* name, GCGameObject* pParent, bool active, const char* tag, int layer );
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
    
    GCScene* m_pScene;
    GCGameObject* m_pParent;
    GCVector<GCGameObject*> m_childrenList;
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
    T* component;
    if ( m_componentsList.Find( T::TYPE, component ) == true )
    {
        delete component;
        m_componentsList.Remove( T::TYPE );
    }
}