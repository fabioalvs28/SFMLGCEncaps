#pragma once
#include "../core/framework.h"

class GCSceneManager
{
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
	
	void CreateGameObject( GCGameObject* pGameObject );
	void DestroyGameObject( GCGameObject* pGameObject );
	
	void AddGameObjectToDeleteQueue( GCGameObject* pGameObject );
	void AddGameObjectToCreateQueue( GCGameObject* pGameObject );
	
	GCScene* CreateScene();

private:
    GCScene* m_pActiveScene;
	GCList<GCScene*> m_scenesList;
	GCList<GCScene*> m_loadedScenesList;
	GCList<GCGameObject*> m_gameObjectsToDeleteList;
	GCList<GCGameObject*> m_gameObjectsToCreateList;
};