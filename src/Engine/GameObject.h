#pragma once
#include "pch.h"



class GCGameObject
{
friend struct GCGameObjectTransform;
friend class GCComponent;
friend class GCScript;
friend class GCScene;
friend class GCSceneManager;
friend class GCPhysicManager;
friend class GCRenderManager;
friend class GCMouseInputManager;
friend class GC;

protected:
    GCGameObject( GCScene* pScene );
    ~GCGameObject() = default;

public:
    void Destroy();
    
    GCGameObject* Duplicate();
    GCGameObject* Duplicate( GCGameObject* pParent );
    
    void RemoveParent();
    GCGameObject* CreateChild();
    void AddChild( GCGameObject* pChild );
    void DestroyChildren();
    
    void AddTag( const char* tag );
    void RemoveTag( const char* tag );
    void RemoveTags();
    bool HasTag( const char* tag ) const;
    
    void Activate();
    void Deactivate();
    
    void SetScene( GCScene* pScene );
    void SetParent( GCGameObject* pParent );
    void SetActive( const bool active );
    void SetName( const char* name );
    void SetLayer( const int layer );
    
    unsigned int GetID() const;
    GCScene* GetScene() const;
    GCGameObject* GetParent() const;
    GCList<GCGameObject*>& GetChildren();
    bool IsActive() const;
    const char* GetName() const;
    int GetLayer() const;
    
    template <class T>
    T* AddComponent();
    template <class T>
    T* GetComponent();
    template <class T>
    void RemoveComponent();
    void ClearComponents();

protected:
    void ActivateGlobal();
    void DeactivateGlobal();
    
    void RemoveScene();
    
    void RegisterScriptToTrigger( GCScript* pScript );
    void OnTriggerEnter( GCCollider* pCollider );
    void OnTriggerStay( GCCollider* pCollider );
    void OnTriggerExit( GCCollider* pCollider );
    
    void RegisterScriptToClicked( GCScript* pScript );
    void OnClick();
    
    void RegisterComponents();
    void UnregisterComponents();

    void RemoveComponent( int ID );

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
    
    bool m_destroyed; // A boolean value indicating if the GameObject will be destroyed the next frame.
    
    bool m_globalActive; // The global active state of the GameObject ( Usually its Parent active state ).
    bool m_selfActive; // The active state of the GameObject.
    
    const char* m_name; // The GameObject's name.
    GCList<const char*> m_tagsList; // The list of tags the GameObject has.
    int m_layer; // The GameObject's layer.
    
    std::map<int, GCComponent*> m_componentsList; // The list of Components the GameObject has.
    GCList<GCScript*> m_scriptTriggerList; // The list of Scripts that will be called when a trigger collision happens with this GameObject
    GCList<GCScript*> m_scriptClickedList; // The list of Scripts that will be called when the mouse clicks on this GameObject

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
	pComponent->Start();
    pComponent->m_globalActive = IsActive();
    m_componentsList.insert( std::pair<int, T*>( T::GetIDStatic(), pComponent ) );
    GC::GetActiveSceneManager()->AddToCreateQueue( pComponent );
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
    std::map<int, GCComponent*>::iterator it;
    it = m_componentsList.find(T::GetIDStatic());
    if ( it != m_componentsList.end() )
        return (T*) it->second;
    return nullptr;
}

//////////////////////////////////////////////////////
/// @brief Removes a Component from the GameObject.
/// 
/// @tparam T The class of the Component to remove.
//////////////////////////////////////////////////////
template <class T>
void GCGameObject::RemoveComponent() { RemoveComponent( T::GetIDStatic() ); }