#pragma once
#include "List.h"

// TODO Scripts in Scenes

class GCScene
{
friend class GCGameObject;
friend class GCSceneManager;
friend class GC;

protected:
	GCScene();
	virtual ~GCScene() = default;

public:
	static GCScene* Create();
	void Load();
	void Unload();
	
	void RemoveParent();
	GCScene* CreateChild();
	void AddChild( GCScene* pScene );
	void DestroyChildren();
	
	GCGameObject* CreateGameObject();
	GCGameObject* CreateGameObject( GCGameObject* pParent );
	GCGameObject* FindGameObjectByName( const char* name );
	GCGameObject* FindGameObjectByID( int ID );
	void DestroyGameObjects();
	
	void SetParent( GCScene* pParent );
	
	GCScene* GetParent() const;

protected:
	void Destroy();
	
	void MoveGameObjectToScene( GCGameObject* pGameObject );
	void RemoveGameObjectFromScene( GCGameObject* pGameObject );

protected:
	GCListNode<GCScene*>* m_pNode;
	GCListNode<GCScene*>* m_pLoadedNode;
	GCListNode<GCScene*>* m_pChildNode;
	
	GCScene* m_pParent;
	GCList<GCScene*> m_childrenList;
	
	const char* m_name;
	bool m_active;
	
	GCList<GCGameObject*> m_gameObjectsList;

};