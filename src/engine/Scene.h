#pragma once
#include "List.h"
#include "Map.h"

class Camera;
class Script;

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
	void Destroy();
	
	void RemoveParent();
	GCScene* CreateChild();
	void AddChild( GCScene* pScene );
	void DestroyChildren();
	
	GCGameObject* CreateGameObject();
	GCGameObject* CreateGameObject( GCGameObject* pParent );
	GCGameObject* FindGameObjectByName( const char* name );
	GCGameObject* FindGameObjectByID( int ID );
	void DestroyGameObject( GCGameObject* pGameObject );
	void DestroyGameObjects();
	
	void SetParent( GCScene* pParent );
	GCScene* GetParent() const;
    
	template <class ScriptClass>
    ScriptClass* AddScript();
    template <class ScriptClass>
    ScriptClass* GetScript();
    template <class ScriptClass>
    void RemoveScript();
	
	Camera* GetMainCamera();

protected:
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
	Camera* m_pMainCamera;
	
	GCList<GCGameObject*> m_gameObjectsList;
    GCMap<unsigned int, Script*> m_scriptsList;

};






template <class ScriptClass>
ScriptClass* GCScene::AddScript()
{
    ASSERT( GetScript<ScriptClass>() == nullptr, LOG_FATAL, "Trying to add a Script to a Scene that it already has it" );
    ScriptClass* pScript = new ScriptClass();
    pScript->RegisterToManagers();
    m_scriptsList.Insert( ScriptClass->GetIDStatic(), pScript );
    return pScript;
}

template <class ScriptClass>
ScriptClass* GCScene::GetScript()
{
    ScriptClass* pScript;
    if ( m_scriptsList.Find( ScriptClass::GetIDStatic(), pScript ) == true )
        return (ScriptClass*) pScript;
    return nullptr;
}

template <class ScriptClass>
void GCScene::RemoveScript()
{
    ScriptClass* pScript = GetScript<ScriptClass>();
    ASSERT( pScript != nullptr, LOG_FATAL, "Trying to remove a Script from a Scene that doesn't have it" );
    delete pScript;
    m_scriptsList.Remove( ScriptClass::GetIDStatic() );
}