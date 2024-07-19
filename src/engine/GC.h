#pragma once
#include "List.h"
#include "GameManager.h"

class GCGameManager;
class GCPhysicManager;
class GCUpdateManager;
class GCEventManager;
class GCSceneManager;
class GCRenderManager;

class GC
{
friend class GCGameManager;

private:
    GC() = delete;
    ~GC() = delete;

public:
    template <class MainScript>
    static GCGameManager* CreateGameManager();
    
    static void Destroy( GCGameObject*& pGameObject );
    static void Destroy( GCScene*& pScene );
    
    static GCGameManager* GetActiveGameManager();
    
    static GCPhysicManager* GetActivePhysicManager();
    static GCUpdateManager* GetActiveUpdateManager();
    static GCEventManager* GetActiveEventManager();
    static GCSceneManager* GetActiveSceneManager();
    static GCRenderManager* GetActiveRenderManager();

private:
    inline static GCList<GCGameManager*> m_pGameManagersList = GCList<GCGameManager*>();
    inline static GCGameManager* m_pActiveGameManager = nullptr;

};



template <class MainScript>
GCGameManager* GC::CreateGameManager()
{
    GCGameManager* pGameManager = new GCGameManager();
    pGameManager->m_pNode = m_pGameManagersList.PushBack( pGameManager );
    if ( m_pActiveGameManager == nullptr )
        m_pActiveGameManager = pGameManager;
    return pGameManager;
}