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



void GCScene::CreateGameObject( const char* name = "GameObject", bool active = true, const char* tag = "", int layer = 0 )
{
	GCGameObject* gameObject = new GCGameObject( name, active, tag, layer );
	m_gameObjectsList.PushBack( gameObject );
	gameObject->SetNode( m_gameObjectsList.GetLastNode() );
}

void GCScene::DestroyGameObject( GCGameObject* pGameObject )
{
	GCListNode<GCGameObject*>* pGameObjectNode = pGameObject->GetNode();
	m_gameObjectsList.DeepDeleteNode( pGameObjectNode );
}

void GCScene::DuplicateGameObject( GCGameObject* pGameObject )
{
	CreateGameObject( pGameObject->GetName(), pGameObject->IsActive(), pGameObject->GetTag(), pGameObject->GetLayer() );
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