#pragma once
#include "../Core/framework.h"

class GCSceneManager;
class GCGameObject;

class GCScene
{
friend class GCSceneManager;

public:
	void Destroy();
	void Load();
	void Unload();
	
	void SetActive( bool active ) { m_active = active; }
	
	bool IsActive() { return m_active; }
	
	void CreateGameObject( const char* name /*= GameObject*/, bool active /*= true*/, const char* tag /*= ""*/, int layer /*= 0*/ );
	void DestroyGameObject( GCGameObject* pGameObject );

	void DuplicateGameObject( GCGameObject* pGameObject ); 
	void MoveGameObjectToScene( GCScene* pScene, GCGameObject* pGameObject ); 

private:
	GCScene();
	virtual ~GCScene() {}
	void Update();
	void Render();
	
	GCListNode<GCScene*>* GetNode() { return m_pNode; }
	GCListNode<GCScene*>* GetLoadedNode() { return m_pLoadedNode; }
	
	void SetNode( GCListNode<GCScene*>* pNode ) { m_pNode = pNode; }
	void SetLoadedNode( GCListNode<GCScene*>* pLoadedNode ) { m_pLoadedNode = pLoadedNode; }
	void RemoveLoadedNode() { m_pLoadedNode = nullptr; }
	
	GCGameObject* RemoveGameObjectFromScene( GCGameObject* pGameObject ); 

private:
	GCListNode<GCScene*>* m_pNode;
	GCListNode<GCScene*>* m_pLoadedNode;
	bool m_active; 
	GCList<GCGameObject*> m_gameObjectsList;

};