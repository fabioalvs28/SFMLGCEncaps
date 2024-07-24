#pragma once
#include "Map.h"
#include "InputManager.h"
#include "EventManager.h"
#include "PhysicManager.h"
#include "UpdateManager.h"
#include "SceneManager.h"
#include "RenderManager.h"

template <typename T>
class GCListNode;
class Script;
struct HINSTANCE__;
typedef struct HINSTANCE__ *HINSTANCE;

class GCGameManager
{
    friend class GC;

public:
    void Run();
    
    template <class ScriptClass>
    ScriptClass* AddScript();
    template <class ScriptClass>
    ScriptClass* GetScript();
    template <class ScriptClass>
    void RemoveScript();

protected:
    GCGameManager( HINSTANCE hInstance );
    ~GCGameManager() = default;

    void GameLoop();

    void SetActiveGameManager();

protected:
    GCListNode<GCGameManager*>* m_pNode;
    Window* m_pWindow;
    
    GCInputSystem m_inputSystem;
    GCEventManager m_eventManager;
    GCPhysicManager m_physicManager;
    GCUpdateManager m_updateManager;
    GCSceneManager m_sceneManager;
    GCRenderManager m_renderManager;
    
    GCMap<unsigned int, Script*> m_scriptsList;
};






template <class ScriptClass>
ScriptClass* GCGameManager::AddScript()
{
    ASSERT( GetScript<ScriptClass>() == nullptr, LOG_FATAL, "Trying to add a Script to the GameManager which it already has it" );
    ScriptClass* pScript = new ScriptClass();
    pScript->RegisterToManagers();
    m_scriptsList.Insert( ScriptClass->GetIDStatic(), pScript );
    return pScript;
}

template <class ScriptClass>
ScriptClass* GCGameManager::GetScript()
{
    ScriptClass* pScript;
    if ( m_scriptsList.Find( ScriptClass::GetIDStatic(), pScript ) == true )
        return (ScriptClass*) pScript;
    return nullptr;
}

template <class ScriptClass>
void GCGameManager::RemoveScript()
{
    ScriptClass* pScript = GetScript<ScriptClass>();
    ASSERT( pScript != nullptr, LOG_FATAL, "Trying to remove a Script from the GameManager which doesn't have it" );
    delete pScript;
    m_scriptsList.Remove( ScriptClass::GetIDStatic() );
}
