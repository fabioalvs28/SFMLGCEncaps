#include "pch.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "../Core/framework.h"



void GCSceneManager::CreateScene()
{
	GCScene* scene = new GCScene();
    m_scenesList.PushBack( scene );
	scene->SetNode( m_scenesList.GetLastNode() );
}

void GCSceneManager::DestroyScene( GCScene* pScene )
{
	GCListNode<GCScene*>* pSceneLoadedNode = pScene->GetLoadedNode();
	if ( pSceneLoadedNode != nullptr ) UnloadScene( pScene );
	GCListNode<GCScene*>* pSceneNode = pScene->GetNode();
	m_loadedScenesList.DeepDeleteNode( pSceneNode );
}

void GCSceneManager::LoadScene( GCScene* pScene )
{
	m_loadedScenesList.PushBack( pScene );
	pScene->SetLoadedNode( m_loadedScenesList.GetLastNode() );
}

void GCSceneManager::UnloadScene( GCScene* pScene )
{
	GCListNode<GCScene*>* pSceneLoadedNode = pScene->GetLoadedNode();
	pScene->RemoveLoadedNode();
	m_loadedScenesList.DeleteNode( pSceneLoadedNode );
}

void GCSceneManager::Update()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
		{
			scene->Update();
			// TODO Update des parents également
		}
	}
}

void GCSceneManager::Render()
{
	GCScene* scene;
	for ( GCListNode<GCScene*>* sceneNode = m_loadedScenesList.GetFirstNode(); sceneNode != m_loadedScenesList.GetLastNode(); sceneNode = sceneNode->GetNext() )
	{
		scene = sceneNode->GetData();
		if ( scene->IsActive() == true )
		{
			scene->Render();
			// TODO Update des parents également
		}
	}
}