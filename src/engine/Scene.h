#pragma once
#include "../Core/framework.h"

class GCSceneManager;
class GCGameObject;

class GCScene
{
friend class GCSceneManager;

public:
	void Destroy();
	void DestroyGameObjectsList();
	void Load();
	void Unload();
	
	void SetActive( bool active ) { m_active = active; }
	void SetParent( GCScene* pParent ) { m_pParent = pParent; }

	bool IsActive() { return m_active; }

	GCScene* GetParent() const { return m_pParent; }
	
	GCGameObject* CreateGameObject( const char* name = "GameObject", bool active = true, const char* tag = "", int layer = 0 );
	void DestroyGameObject( GCGameObject* pGameObject );
	GCGameObject* FindGameObjectByName( const char* name );
	GCGameObject* FindGameObjectByID( int ID );

	void DuplicateGameObject( GCGameObject* pGameObject ); 
	void MoveGameObjectToScene( GCScene* pScene, GCGameObject* pGameObject ); 

	void CreateChild(); 
	void AddChild( GCScene* pScene );
	void DeleteChild(unsigned int childIndex);
	void RemoveChild( GCScene* pChild );
	GCVector<GCScene*> GetChildren() { return m_childrenList; }
	GCScene* GetChild( unsigned int childIndex ) { return m_childrenList.Get( childIndex ); }


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

	GCScene* m_pParent; 
	GCVector<GCScene*> m_childrenList;

};