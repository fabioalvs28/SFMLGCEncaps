#pragma once
#include "../Core/framework.h"

class GCScene;
class GCGameManager;

class GCSceneManager
{
friend class GCGameManager;

public:
	virtual ~GCSceneManager() {}
	
	void Update();
	void Render();
	
	static GCScene* CreateScene();
	static void DestroyScene( GCScene* pScene );
	static void LoadScene( GCScene* pScene );
	static void UnloadScene( GCScene* pScene );

private:
	static GCList<GCScene*> m_scenesList;
	static GCList<GCScene*> m_loadedScenesList;

};
