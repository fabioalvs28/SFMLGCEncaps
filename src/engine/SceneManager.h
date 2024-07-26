#pragma once
#include "../core/framework.h"

class Component;
class GCScene;

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
	
	void CreateComponent( Component* pComponent );
	void DestroyComponent( Component* pComponent );
	
	void AddComponentToCreateQueue( Component* pComponent );
	void AddComponentToDeleteQueue( Component* pComponent );
	
	GCScene* CreateScene();

private:
    GCScene* m_pActiveScene;
	GCList<GCScene*> m_scenesList;
	GCList<GCScene*> m_loadedScenesList;
	GCList<Component*> m_componentsToCreateList;
	GCList<Component*> m_componentsToDeleteList;
};