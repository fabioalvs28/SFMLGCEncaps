#pragma once
#include "../core/framework.h"

class GCGameObject;
class GCScene;
class GCGameManager;

class GCSceneManager
{
friend class GCGameObject;
friend class GCScene;
friend class GCGameManager;

private:
    GCSceneManager() = default;
	virtual ~GCSceneManager() = default;
	
	void Update();
	void NewDelete();
	void Render();
	
	void SetActiveScene( GCScene* pScene );
	void LoadScene( GCScene* pScene );
	void UnloadScene( GCScene* pScene );
	void DestroyScene( GCScene* pScene );
	
	void CreateGameObject( GCGameObject* pGameObject );
	void DestroyGameObject( GCGameObject* pGameObject );
	
	void AddGameObjectToDeleteQueue( GCGameObject* pGameObject );
	void AddGameObjectToCreateQueue( GCGameObject* pGameObject );

protected:
	GCScene* CreateScene();

private:
    GCScene* m_pActiveScene;
	GCList<GCScene*> m_scenesList;
	GCList<GCScene*> m_loadedScenesList;
	GCList<GCGameObject*> m_gameObjectsToDeleteList;
	GCList<GCGameObject*> m_gameObjectsToCreateList;
};