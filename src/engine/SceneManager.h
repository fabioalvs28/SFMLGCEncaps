#pragma once
#include "pch.h"

class GCSceneManager
{
friend class Component;
friend class GCGameObject;
friend class GCScene;
friend class GCGameManager;
friend class GC;

private:
    GCSceneManager() = default;
	virtual ~GCSceneManager() = default;
	
	void Update();
	
	void SetActiveScene( GCScene* pScene );
	GCScene* GetActiveScene();
	
	void LoadScene( GCScene* pScene );
	void UnloadScene( GCScene* pScene );
	void DestroyScene( GCScene* pScene );
	
	void CreateComponent( Component* pComponent );
	void DestroyComponent( Component* pComponent );
	
	void AddComponentToCreateQueue( Component* pComponent );
	void AddComponentToDeleteQueue( Component* pComponent );
	
	GCScene* CreateScene();

private:
    GCScene* m_pActiveScene; // A pointer to the active Scene
	
	GCList<GCScene*> m_scenesList; // A list of pointers to every existing Scenes in the Game
	GCList<GCScene*> m_loadedScenesList; // A list of pointers to every loaded scenes in the Game
	
	GCList<Component*> m_componentsToCreateList; // A list of pointers to the Components to create in the next frame
	GCList<Component*> m_componentsToDeleteList; // A list of pointers to the Components to delete in the next frame
};