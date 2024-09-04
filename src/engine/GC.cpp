#include "pch.h"





GCGameManager* GC::CreateGameManager( HINSTANCE hInstance )
{
    GCGameManager* pGameManager = new GCGameManager( hInstance );
    pGameManager->m_pNode = m_gameManagersList.PushBack( pGameManager );
    if ( m_pActiveGameManager == nullptr )
        m_pActiveGameManager = pGameManager;
    return pGameManager;
}



GCGameManager* GC::GetActiveGameManager()
{ return m_pActiveGameManager; }

Window* GC::GetWindow()
{ return m_pActiveGameManager->m_pWindow; }

GCTime* GC::GetActiveTimer()
{ return m_pActiveGameManager->m_pTimer; }

GCInputSystem* GC::GetActiveInputSystem()
{ return m_pActiveGameManager->m_pInputSystem; }

GCEventManager* GC::GetActiveEventManager()
{ return m_pActiveGameManager->m_pEventManager; }

GCPhysicManager* GC::GetActivePhysicManager()
{ return m_pActiveGameManager->m_pPhysicManager; }

GCUpdateManager* GC::GetActiveUpdateManager()
{ return m_pActiveGameManager->m_pUpdateManager; }

GCSceneManager* GC::GetActiveSceneManager()
{ return m_pActiveGameManager->m_pSceneManager; }

GCScene* GC::GetActiveScene()
{ return m_pActiveGameManager->m_pSceneManager->m_pActiveScene; }

GCRenderManager* GC::GetActiveRenderManager()
{ return m_pActiveGameManager->m_pRenderManager; }

GCTextManager* GC::GetActiveTextManager()
{ return m_pActiveGameManager->m_pTextManager; }
