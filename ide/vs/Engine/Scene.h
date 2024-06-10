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
	
	GCGameObject* CreateGameObject( const char* name /*= GameObject*/, bool active /*= true*/ );

private:
	GCScene() {}
	virtual ~GCScene() {}
	void Update();
	
	GCListNode<GCScene*>* GetNode() { return m_pNode; }
	GCListNode<GCScene*>* GetLoadedNode() { return m_pLoadedNode; }
	
	void SetNode( GCListNode<GCScene*>* pNode ) { m_pNode = pNode; }
	void SetLoadedNode( GCListNode<GCScene*>* pLoadedNode ) { m_pLoadedNode = pLoadedNode; }
	void RemoveLoadedNode() { m_pLoadedNode = nullptr; }

private:
	GCListNode<GCScene*>* m_pNode;
	GCListNode<GCScene*>* m_pLoadedNode;
	bool m_active; 
	GCList<GCGameObject*> m_gameObjectsList;

};