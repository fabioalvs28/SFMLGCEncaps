#include "pch.h"
#include "GameObject.h"

#include "Components.h"
#include "Scene.h"
#include "SceneManager.h"

// todo Children inheriting from the parent m_active
// todo GameObject being able to have multiple tags



GCGameObject::GCGameObject( GCScene* pScene )
{
    m_ID = s_gameObjectsCount++;
    m_pSceneNode = nullptr;
    m_pChildNode = nullptr;
    
    m_pScene = pScene;
    m_pParent = nullptr;
    
    m_created = false;
    m_active = true;
    m_name = "GameObject";
    m_tag = "";
    m_layer = 0;
}

void GCGameObject::Update()
{
    if ( m_active == false || m_created == false ) return;
    Component* component; 
    for (int i = 1 ; i <= 7 ; i++)
        if ( m_componentsList.Find(i, component) == true ) 
            if ( component->m_active == true )
                component->Update();
}

void GCGameObject::Render()
{
    if ( m_active == false || m_created == false ) return;
    // todo GameObject Render
}

void GCGameObject::Destroy()
{
    GCSceneManager::AddGameObjectToDeleteQueue( this );
}



void GCGameObject::SetParent( GCGameObject* pParent )
{
    pParent->AddChild( this );
}

void GCGameObject::RemoveParent()
{
    if ( m_pParent == nullptr ) return;
    m_pParent->RemoveChild( this );
}

GCGameObject* GCGameObject::CreateChild()
{
    GCGameObject* pChild = m_pScene->CreateGameObject();
    AddChild( pChild ); //! Not really efficient ( verifications could be skipped as the GameObject was just created )
    return pChild;
}

void GCGameObject::AddChild( GCGameObject* pChild )
{
    if ( pChild == this ) return;
    if ( pChild->m_pParent == this ) return;
    for ( GCGameObject* pAncestor = pChild->m_pParent; pAncestor != nullptr; pAncestor = pAncestor->m_pParent )
        if ( pChild == pAncestor ) return;
    
    pChild->RemoveParent();
    m_childrenList.PushBack( pChild );
    pChild->m_pChildNode = m_childrenList.GetLastNode();
    pChild->m_pParent = this;
    // todo Updating the transform so that the GameObject stays where it was before it was added
    // todo Assert for the errors instead of simply returning nothing
}

void GCGameObject::RemoveChild( GCGameObject* pChild )
{
    if ( pChild->m_pParent != this ) return;
    
    m_childrenList.RemoveNode( pChild->m_pChildNode );
    pChild->m_pChildNode = nullptr;
    pChild->m_pParent = nullptr;
    // todo Updating the transform so that the GameObject stays where it was before it was removed
    // todo Assert for the errors instead of simply returning nothing
}

void GCGameObject::DeleteChildren()
{
    for ( GCListNode<GCGameObject*>* pChildNode = m_childrenList.GetFirstNode(); pChildNode != nullptr; pChildNode = pChildNode->GetNext() )
        pChildNode->GetData()->Destroy();
    m_childrenList.Clear();
}



void GCGameObject::SetActive( bool active ) { m_active = active; }

void GCGameObject::SetName( const char* name ) { m_name = name; }

void GCGameObject::SetTag( const char* tag ) { m_tag = tag; }

void GCGameObject::SetLayer( int layer ) { m_layer = layer; }



unsigned int GCGameObject::GetID() const { return m_ID; }

GCScene* GCGameObject::GetScene() const { return m_pScene; }

GCGameObject* GCGameObject::GetParent() const { return m_pParent; }

GCList<GCGameObject*> GCGameObject::GetChildren() const { return m_childrenList; }

bool GCGameObject::IsActive() const { return m_active; }

const char* GCGameObject::GetName() const { return m_name; }

const char* GCGameObject::GetTag() const { return m_tag; }

int GCGameObject::GetLayer() const { return m_layer; }



void GCGameObject::RemoveComponent( int type )
{
    Component* component;
    if ( m_componentsList.Find( type, component ) == true )
    {
        delete component;
        m_componentsList.Remove( type );
    }
    // todo Try to think of another way to store components
}

void GCGameObject::ClearComponents()
{
    for ( int i = 1; i <= 7; i++ ) RemoveComponent( i ); //! Limited to 7 components
    // todo Try to think of another way to store components
}