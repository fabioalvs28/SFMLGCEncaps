#include "pch.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "../Core/framework.h"



void GCSceneManager::Update()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
			scene->Update();
	}
}

void GCSceneManager::Render()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
			scene->Render();
	}
}




GCScene* GCSceneManager::CreateScene()
{
	GCScene* scene = new GCScene();
    m_scenesList.PushBack( scene );
	scene->SetNode( m_scenesList.GetLastNode() ); 
	return scene;
}

void GCSceneManager::LoadScene( GCScene* pScene )
{
	if ( pScene->GetLoadedNode() != nullptr ) return;
	m_loadedScenesList.PushBack( pScene );
	pScene->SetLoadedNode( m_loadedScenesList.GetLastNode() );
}

void GCSceneManager::UnloadScene( GCScene* pScene )
{
	GCListNode<GCScene*>* pLoadedNode = pScene->GetLoadedNode();
	if ( pLoadedNode == nullptr ) return;
	m_loadedScenesList.DeleteNode( pLoadedNode );
	pScene->RemoveLoadedNode();
}

void GCSceneManager::DestroyScene( GCScene* pScene )
{
	UnloadScene( pScene );
	m_scenesList.DeleteNode( pScene->GetNode() );
	pScene->ClearGameObjects();
	pScene->ClearChildren();
    delete pScene;
}