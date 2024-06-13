#pragma once
#include "../core/framework.h"

class GCGameManager;
class GCScene;
class GCGameObject;

class GCSceneManager
{
friend class GCGameManager;
friend class GCScene;
friend class GCGameObject;

public:
	static GCScene* CreateScene( GCScene* pParent = nullptr );

private:
    GCSceneManager() {}
	virtual ~GCSceneManager() {}
	
	static void Update();
	static void NewDelete();
	static void Render();
	
	static void LoadScene( GCScene* pScene );
	static void UnloadScene( GCScene* pScene );
	static void DestroyScene( GCScene* pScene );
	
	static void AddGameObjectToDeleteQueue( GCGameObject* pGameObject );
	static void AddSceneToDeleteQueue( GCScene* pScene );

private:
	static GCList<GCScene*> m_scenesList;
	static GCList<GCScene*> m_loadedScenesList;
	static GCList<GCGameObject*> m_gameObjectsToDeleteList;
    static GCList<GCScene*> m_scenesToDeleteList;
};