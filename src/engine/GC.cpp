#include "pch.h"
#include "GC.h"

#include "GameObject.h"
#include "Scene.h"





GCGameManager* GC::CreateGameManager( HINSTANCE hInstance )
{
    GCGameManager* pGameManager = new GCGameManager( hInstance );
    pGameManager->m_pNode = m_pGameManagersList.PushBack( pGameManager );
    if ( m_pActiveGameManager == nullptr )
        m_pActiveGameManager = pGameManager;
    return pGameManager;
}



GCGameManager* GC::GetActiveGameManager()
{ return m_pActiveGameManager; }

Window* GC::GetWindow()
{ return m_pActiveGameManager->m_pWindow; }

GCInputSystem* GC::GetActiveInputSystem()
{
    return m_pActiveGameManager->m_pInputSystem;
}

GCTime* GC::GetActiveTimer()
{
    return m_pActiveGameManager->m_Time;
}

GCEventManager* GC::GetActiveEventManager()
{ return m_pActiveGameManager->m_pEventManager; }

GCPhysicManager* GC::GetActivePhysicManager()
{ return m_pActiveGameManager->m_pPhysicManager; }

GCUpdateManager* GC::GetActiveUpdateManager()
{ return m_pActiveGameManager->m_pUpdateManager; }

GCSceneManager* GC::GetActiveSceneManager()
{ return m_pActiveGameManager->m_pSceneManager; }

GCRenderManager* GC::GetActiveRenderManager()
{ return m_pActiveGameManager->m_pRenderManager; }
