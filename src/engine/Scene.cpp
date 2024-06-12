#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Components.h"


GCScene::GCScene()
{
	m_active = false;
}

void GCScene::Destroy()
{
	GCSceneManager::DestroyScene( this );
}

void GCScene::Load()
{
	GCSceneManager::LoadScene( this );
}

void GCScene::Unload()
{
	GCSceneManager::UnloadScene( this );
}



void GCScene::Update()
{
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != m_gameObjectsList.GetLastNode(); pGameObjectNode = pGameObjectNode->GetNext() )
		pGameObjectNode->GetData()->Update();
}

void GCScene::Render()
{
	SpriteRenderer* pSpriteRenderer;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != m_gameObjectsList.GetLastNode(); pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pSpriteRenderer = pGameObjectNode->GetData()->GetComponent<SpriteRenderer>();
		if ( pSpriteRenderer != nullptr )
		    pSpriteRenderer->Render();
	}
}



GCGameObject* GCScene::CreateGameObject( const char* name = "GameObject", GCGameObject* mParent = nullptr, bool active = true, const char* tag = "", int layer = 0 )
{
	GCGameObject* pGameObject = new GCGameObject( this, name, mParent, active, tag, layer );
	m_gameObjectsList.PushBack( pGameObject );
	pGameObject->SetNode( m_gameObjectsList.GetLastNode() );
	return pGameObject;
}

void GCScene::DestroyGameObject( GCGameObject* pGameObject )
{
	GCListNode<GCGameObject*>* pGameObjectNode = pGameObject->GetNode();
	m_gameObjectsList.DeleteNode( pGameObjectNode );
	delete pGameObject;
}

GCGameObject* GCScene::FindGameObjectByName( const char* name )
{
	GCGameObject* pGameObject;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode!= m_gameObjectsList.GetLastNode(); pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pGameObject = pGameObjectNode->GetData();
		if ( pGameObject->GetName() == name )
			return pGameObject;
	}
}

GCGameObject* GCScene::FindGameObjectByID( int ID )
{
	GCGameObject* pGameObject;
	for ( GCListNode<GCGameObject*>* pGameObjectNode = m_gameObjectsList.GetFirstNode(); pGameObjectNode != m_gameObjectsList.GetLastNode(); pGameObjectNode = pGameObjectNode->GetNext() )
	{
		pGameObject = pGameObjectNode->GetData();
		if ( pGameObject->GetID() == ID )
            return pGameObject;
	}
}

void GCScene::DuplicateGameObject( GCGameObject* pGameObject )
{
	CreateGameObject( pGameObject->GetName(), pGameObject->GetParent() , pGameObject->IsActive(), pGameObject->GetTag(), pGameObject->GetLayer() );
}

GCGameObject* GCScene::RemoveGameObjectFromScene( GCGameObject* pGameObject )
{
	GCListNode<GCGameObject*>* pGameObjectNode = pGameObject->GetNode();
    m_gameObjectsList.DeleteNode( pGameObjectNode );
	return pGameObject;
}

void GCScene::MoveGameObjectToScene( GCScene* pScene, GCGameObject* pGameObject )
{ 
	RemoveGameObjectFromScene( pGameObject );
	pScene->m_gameObjectsList.PushBack( pGameObject ); 
	pGameObject->SetNode( pScene->m_gameObjectsList.GetLastNode() );
}


// TODO PREFAB