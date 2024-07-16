#pragma once
#include "List.h"

class GCGameObject;
class GCSceneManager;

class GCScene
{
friend class GCGameObject;
friend class GCSceneManager;

protected:
	GCScene();
	virtual ~GCScene() = default;
	
	void Update();
	void Render();

public:
	static GCScene* Create();
	void Load();
	void Unload();
	void Destroy();
	
	void RemoveParent();
	GCScene* CreateChild();
	void AddChild( GCScene* pScene );
	void RemoveChild( GCScene* pChild );
	void DestroyChildren();
	
	GCGameObject* CreateGameObject();
	static GCGameObject* DuplicateGameObject( GCGameObject* pGameObject );
	GCGameObject* FindGameObjectByName( const char* name );
	GCGameObject* FindGameObjectByID( int ID );
	void DestroyGameObjects();
	
	void SetParent( GCScene* pParent );
	
	GCScene* GetParent() const;

protected:
	void MoveGameObjectToScene( GCGameObject* pGameObject );
	void RemoveGameObjectFromScene( GCGameObject* pGameObject );
	void DestroyGameObject( GCGameObject* pGameObject );

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