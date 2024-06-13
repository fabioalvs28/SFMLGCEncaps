#pragma once
#include "../Core/framework.h"

class GCScene;
class GCGameManager;

class GCSceneManager
{
friend class GCGameManager;
friend class GCScene;

public:
	static GCScene* CreateScene();

private:
    GCSceneManager() {}
	virtual ~GCSceneManager() {}
	
	static void Update();
	static void Render();
	
	static void LoadScene( GCScene* pScene );
	static void UnloadScene( GCScene* pScene );
	static void DestroyScene( GCScene* pScene );

private:
	static GCList<GCScene*> m_scenesList;
	static GCList<GCScene*> m_loadedScenesList;

};