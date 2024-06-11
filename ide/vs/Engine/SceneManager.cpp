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

// void GCSceneManager::Update() {

// 	for (GCListNode<GCScene*>* sceneNode = m_loadedSceneList.GetFirstNode(); sceneNode != m_loadedSceneList.GetLastNode(); sceneNode = sceneNode->GetNext())
// 		sceneNode->GetData()->Update();
// }

// void GCSceneManager::RenderScene()
// {
// 	for (GCListNode<GCScene*>* sceneNode = m_sceneList.GetFirstNode(); sceneNode != m_sceneList.GetLastNode(); sceneNode = sceneNode->GetNext())
// 		if (sceneNode->GetData()->m_isActive == true)
// 		{
// 			// Render. 
// 		}
// }