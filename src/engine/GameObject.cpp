#include "pch.h"
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"



GCGameObject::GCGameObject( GCScene* pScene )
{
    m_ID = s_nextID++;
    m_pNode = nullptr;
    
    m_name = "";
    m_active = true;
    m_tag = "";
    m_layer = 0;
    
    m_pScene = pScene;
    m_pParent = nullptr;
}

GCGameObject::GCGameObject( GCScene* pScene, const char* name, GCGameObject* pParent, bool active, const char* tag, int layer ) 
{
    m_ID = s_nextID++;
    m_pNode = nullptr;
    
    m_name = name;
    m_active = active;
    m_tag = tag;
    m_layer = layer;
    
    m_pScene = pScene;
    m_pParent = pParent;
}

void GCGameObject::SetParent( GCGameObject* pParent )
{
    m_pParent->RemoveChild( this );
    pParent->AddChild( this );
}

void GCGameObject::CreateChild()
{
    AddChild( m_pScene->CreateGameObject() );
}

void GCGameObject::CreateChild( const char* name = "GameObject", bool active = true, const char* tag = "", int layer = 0 )
{
    AddChild( m_pScene->CreateGameObject( name, active, tag, layer ) );
}

void GCGameObject::AddChild( GCGameObject* pChild )
{
    m_childrenList.PushBack( pChild );
    pChild->m_pParent = m_pParent;
}

void GCGameObject::DeleteChild( unsigned int childIndex )
{
    m_childrenList.Get( childIndex )->Destroy();
    m_childrenList.Remove( childIndex );
}

void GCGameObject::RemoveChild( GCGameObject* pChild )
{
    int childIndex; 
    childIndex = m_childrenList.GetIndex( pChild );
    m_childrenList.Remove( childIndex );
}

void GCGameObject::MoveChild( unsigned int childIndex, unsigned int newIndex )
{
    GCGameObject* pChild = m_childrenList.Get( childIndex );
    m_childrenList.Remove( childIndex );
    m_childrenList.Insert( newIndex, pChild );
}

void GCGameObject::Update()
{
    Component* component; 
    for (int i = 1 ; i <= 7 ; i++)
        if ( m_componentsList.Find(i, component) == true ) 
            component->Update();
}

void GCGameObject::Destroy()
{
    m_pScene->DestroyGameObject( this );
}