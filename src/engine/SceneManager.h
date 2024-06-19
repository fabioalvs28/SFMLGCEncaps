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
	
	void LoadScene( GCScene* pScene );
	void UnloadScene( GCScene* pScene );
	void DestroyScene( GCScene* pScene );
	
	void AddGameObjectToDeleteQueue( GCGameObject* pGameObject );
	void AddSceneToDeleteQueue( GCScene* pScene );

protected:
	GCScene* CreateScene();

private:
	GCList<GCScene*> m_scenesList;
	GCList<GCScene*> m_loadedScenesList;
	GCList<GCGameObject*> m_gameObjectsToDeleteList;
    GCList<GCScene*> m_scenesToDeleteList;
};