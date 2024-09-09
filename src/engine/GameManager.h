#pragma once
#include "pch.h"

class Window;
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
    GCListNode<GCGameManager*>* m_pNode; // A pointer to the GameManager's Node in the GC gameManagersList
    Window* m_pWindow; // A pointer to the GameManager's Window
    
    GCTime* m_pTimer; // A pointer to the GameManager's Timer
    GCInputSystem* m_pInputSystem; // A pointer to the GameManager's InputSystem
    GCEventManager* m_pEventManager; // A pointer to the GameManager's EventManager
    GCPhysicManager* m_pPhysicManager; // A pointer to the GameManager's PhysicManager
    GCUpdateManager* m_pUpdateManager; // A pointer to the GameManager's UpdateManager
    GCSceneManager* m_pSceneManager; // A pointer to the GameManager's SceneManager
    GCRenderManager* m_pRenderManager; // A pointer to the GameManager's RenderManager
    GCSpriteSheetHandler* m_pSpriteSheetHandler; // A pointer to the GameManager's RenderManager

    GCMap<unsigned int, GCScript*> m_scriptsList; // A list of pointers to the GameManager's Scripts
};






//////////////////////////////////////////////////////////////////////
/// @brief Adds a new Script to the GameManager.
/// 
/// @tparam ScriptClass The Script class to add to the GameManager.
/// 
/// @return A pointer to the newly created Script.
//////////////////////////////////////////////////////////////////////
template <class ScriptClass>
ScriptClass* GCGameManager::AddScript()
{
    ASSERT( GetScript<ScriptClass>() == nullptr, LOG_FATAL, "Trying to add a Script to the GameManager which it already has it" );
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
ScriptClass* GCGameManager::GetScript()
{
    GCScript* pScript;
    if ( m_scriptsList.Find( ScriptClass::GetIDStatic(), pScript ) == true )
        return static_cast<ScriptClass*>( pScript );
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////
/// @brief Removes a Script from the GameManager.
/// 
/// @tparam ScriptClass The Script class to remove from the GameManager.
///////////////////////////////////////////////////////////////////////////
template <class ScriptClass>
void GCGameManager::RemoveScript()
{
    ScriptClass* pScript = GetScript<ScriptClass>();
    ASSERT( pScript != nullptr, LOG_FATAL, "Trying to remove a Script from the GameManager which doesn't have it" );
    delete pScript;
    m_scriptsList.Remove( ScriptClass::GetIDStatic() );
}
