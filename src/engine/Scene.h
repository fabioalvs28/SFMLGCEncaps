#pragma once
#include "pch.h"

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
	void SetActive();
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
	void DestroyGameObjects();
	
	void SetParent( GCScene* pParent );
	GCScene* GetParent() const;
    
	template <class ScriptClass>
    ScriptClass* AddScript();
    template <class ScriptClass>
    ScriptClass* GetScript();
    template <class ScriptClass>
    void RemoveScript();
	
	GCCamera* GetMainCamera();

protected:
	void Deactivate();
	void MoveGameObjectToScene( GCGameObject* pGameObject );
	void RemoveGameObjectFromScene( GCGameObject* pGameObject );

protected:
	GCListNode<GCScene*>* m_pNode; // A pointer to the Scene's Node in the SceneManager's scenesList
	GCListNode<GCScene*>* m_pLoadedNode; // A pointer to the Scene's Node in the SceneManager's loadedScenesList
	GCListNode<GCScene*>* m_pChildNode; // A pointer to the Scene's Node in its Parent's childrenList
	
	GCScene* m_pParent; // A pointer to the Scene's Parent Scene
	GCList<GCScene*> m_childrenList; // A list of pointers to the Scene's children Scenes
	
	const char* m_name; // The Scene's name
	bool m_active; // Whether or not this Scene is the active Scene
	GCCamera* m_pMainCamera; // A pointer to the Scene's Main Camera
	
	GCList<GCGameObject*> m_gameObjectsList; // A list of pointers to the Scene's GameObjects without Parents
    GCMap<unsigned int, GCScript*> m_scriptsList; // A list of pointers to the Scene's Scripts

};






////////////////////////////////////////////////////////////////
/// @brief Adds a new Script to the Scene.
/// 
/// @tparam ScriptClass The Script class to add to the Scene.
/// 
/// @return A pointer to the newly created Script.
////////////////////////////////////////////////////////////////
template <class ScriptClass>
ScriptClass* GCScene::AddScript()
{
    ASSERT( GetScript<ScriptClass>() == nullptr, LOG_FATAL, "Trying to add a Script to a Scene that it already has it" );
    ScriptClass* pScript = new ScriptClass();
	pScript->Start();
    m_scriptsList.Insert( ScriptClass::GetIDStatic(), pScript );
    GC::GetActiveSceneManager()->AddToCreateQueue( pScript );
    return pScript;
}

////////////////////////////////////////////////////////////
/// @tparam ScriptClass The Script class you want to get.
/// 
/// @return A pointer to the searched Script.
////////////////////////////////////////////////////////////
template <class ScriptClass>
ScriptClass* GCScene::GetScript()
{
    GCScript* pScript;
    if ( m_scriptsList.Find( ScriptClass::GetIDStatic(), pScript ) == true )
        return static_cast<ScriptClass*>(pScript);
    return nullptr;
}

/////////////////////////////////////////////////////////////////////
/// @brief Removes a Script from the Scene.
/// 
/// @tparam ScriptClass The Script class to remove from the Scene.
/////////////////////////////////////////////////////////////////////
template <class ScriptClass>
void GCScene::RemoveScript()
{
    ScriptClass* pScript = GetScript<ScriptClass>();
    ASSERT( pScript != nullptr, LOG_FATAL, "Trying to remove a Script from a Scene that doesn't have it" );
    delete pScript;
    m_scriptsList.Remove( ScriptClass::GetIDStatic() );
}