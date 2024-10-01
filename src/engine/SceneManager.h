#pragma once
#include "pch.h"

class GCSceneManager
{
friend class GCComponent;
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
	
	void CreateComponent( GCComponent* pComponent );
	void DestroyComponent( GCComponent* pComponent );
	
	void DestroyGameObject( GCGameObject* pGameObject );
	
	void AddToCreateQueue( GCComponent* pComponent );
	void AddToDeleteQueue( GCComponent* pComponent );
	
	void AddToDeleteQueue( GCGameObject* pGameObject );
	
	GCScene* CreateScene();

private:
	GCScene* m_pNewActiveScene;
    GCScene* m_pActiveScene; // A pointer to the active Scene
	
	GCList<GCScene*> m_scenesList; // A list of pointers to every existing Scenes in the Game
	GCList<GCScene*> m_loadedScenesList; // A list of pointers to every loaded scenes in the Game
	
	GCList<GCComponent*> m_componentsToCreateList; // A list of pointers to the Components to create in the next frame
	GCList<GCComponent*> m_componentsToDeleteList; // A list of pointers to the Components to delete in the next frame
	
	GCList<GCGameObject*> m_gameObjectsToDeleteList; // A list of pointers of the GameObjects to delete in the next frame
};