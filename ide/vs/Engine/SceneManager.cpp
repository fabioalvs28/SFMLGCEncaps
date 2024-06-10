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

void GCSceneManager::DestroyScene( GCScene* scene )
{
	// TODO Destroy Scene
}

void GCSceneManager::LoadScene( GCScene* scene )
{
	m_loadedScenesList.PushBack( scene );
	scene->SetLoadedNode( m_loadedScenesList.GetLastNode() );
}

void GCSceneManager::UnloadScene( GCScene* scene )
{
	GCListNode<GCScene*>* pSceneLoadedNode = scene->GetLoadedNode();
	pSceneLoadedNode->Remove();
	scene->RemoveLoadedNode();
	// TODO Unloead Scene
}

// void GCSceneManager::CreateScene()
// {
// 	GCScene* scene = new GCScene( m_loadedSceneList );
// 	m_sceneList.PushBack( scene );
// 	scene->m_allSceneNode = m_sceneList.GetLastNode();
// }

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