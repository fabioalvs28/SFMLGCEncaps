#pragma once
#include "../core/framework.h"

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
    ~GCGameObject() {}
    
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
    GCTransform m_transform;
    GCMap<int, Component*> m_componentsList;

};

// <summary>
// This function creates a new instance of the specified component type, initializes it with the game object, and adds it to the game object's component list.
// If the component type already exists on the game object, the function returns nullptr.
// The component's memory is managed internally, and it will be automatically deleted when the game object is destroyed or when the component is removed.
// It also returns a pointer to the newly created component.
// </summary>
// <template param name="T"> The type of the component to be added. </template param>
template<class T>
T* GCGameObject::AddComponent()
{
    if ( GetComponent<T>() != nullptr ) return nullptr;
    T* component = new T();
    component.SetGameObject( this );
    m_componentsList.Insert( T::TYPE, component );
    return component;
}

// <summary>
// This function searches the game object's component list for a component of the specified type.
// If the component is found, it returns a pointer to the component.
// If the component is not found, it returns nullptr.
// The returned pointer should not be used to delete the component, as there is a function to do it.
// </summary>
// <template param name="T"> The type of the component to be retrieved. </template param>
template<class T>
T* GCGameObject::GetComponent()
{
    Component* component;
    if ( m_componentsList.Find( T::TYPE, component ) == true )
        return (T*) component;
    return nullptr;
}

// <summary>
// This function searches the game object's component list for a component of the specified type.
// If the component is found, it is removed from the list and deleted.
// The component's memory is managed internally, and it will be automatically deleted.
// </summary>
// <template param name="T"> The type of the component to be removed. </template param>
template<class T>
void GCGameObject::RemoveComponent()
{
    RemoveComponent( T::TYPE );
}