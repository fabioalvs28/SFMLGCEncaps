#pragma once
#include "../Core/framework.h"

class GCSceneManager;
class GCGameObject;

class GCScene
{
friend class GCSceneManager;
friend class GCGameObject;

public:
	void Destroy();
	void Load();
	void Unload();
	
	GCGameObject* CreateGameObject( const char* name = "GameObject", bool active = true, const char* tag = "", int layer = 0 );
	void DuplicateGameObject( GCGameObject* pGameObject ); 
	GCGameObject* FindGameObjectByName( const char* name );
	GCGameObject* FindGameObjectByID( int ID );	void MoveGameObjectToScene( GCScene* pScene, GCGameObject* pGameObject ); 
	void ClearGameObjects();
	
	void CreateChild(); 
	void AddChild( GCScene* pScene );
	void RemoveChild( GCScene* pChild );
	void DeleteChild( GCScene* pChild );
	void ClearChildren();
	
	void SetActive( bool active ) { m_active = active; }
	void SetParent( GCScene* pParent ) { m_pParent = pParent; }
	
	bool IsActive() { return m_active; }
	GCScene* GetParent() const { return m_pParent; }

private:
	GCScene();
	GCScene( GCScene* pParent );
	virtual ~GCScene() {}
	void Update();
	void Render();
	
	GCGameObject* RemoveGameObjectFromScene( GCGameObject* pGameObject );
	void DestroyGameObject( GCGameObject* pGameObject );
	
	GCListNode<GCScene*>* GetNode() { return m_pNode; }
	GCListNode<GCScene*>* GetLoadedNode() { return m_pLoadedNode; }
	GCListNode<GCScene*>* GetChildNode() { return m_pChildNode; }
	
	void SetNode( GCListNode<GCScene*>* pNode ) { m_pNode = pNode; }
	void SetLoadedNode( GCListNode<GCScene*>* pLoadedNode ) { m_pLoadedNode = pLoadedNode; }
	void SetChildNode( GCListNode<GCScene*>* pChildNode ) { m_pChildNode = pChildNode; }
	
	void RemoveLoadedNode() { m_pLoadedNode = nullptr; }
	void RemoveChildNode() { m_pChildNode = nullptr; }

private:
	GCListNode<GCScene*>* m_pNode;
	GCListNode<GCScene*>* m_pLoadedNode;
	GCListNode<GCScene*>* m_pChildNode;
	
	GCScene* m_pParent;
	GCList<GCScene*> m_childrenList;
	
	bool m_active;
	GCList<GCGameObject*> m_gameObjectsList;

};