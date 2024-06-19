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
	
	void DestroyGameObject( GCGameObject* pGameObject );
	
	void AddGameObjectToDeleteQueue( GCGameObject* pGameObject );

protected:
	GCScene* CreateScene();

private:
    GCScene* m_pActiveScene;
	GCList<GCScene*> m_scenesList;
	GCList<GCScene*> m_loadedScenesList;
	GCList<GCGameObject*> m_gameObjectsToDeleteList;
};