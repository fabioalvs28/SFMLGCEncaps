#include "pch.h"
#include "GameManager.h"

#include "../Render/pch.h"

// TODO Timer Implementation

GCGameManager::GCGameManager( HINSTANCE hInstance )
{
    m_pNode = nullptr;
    m_pWindow = new Window( hInstance );
    m_pWindow->Initialize();
    
    m_pInputSystem = new GCInputSystem();
    m_pEventManager = new GCEventManager();
    m_pPhysicManager = new GCPhysicManager();
    m_pUpdateManager = new GCUpdateManager();
    m_pSceneManager = new GCSceneManager();
    m_pRenderManager = new GCRenderManager( m_pWindow );
}



void GCGameManager::Run()
{
    //! Stop the GameLoop from the old active GameManager if there is one
    SetActiveGameManager();
    GameLoop();
}

void GCGameManager::GameLoop()
{
    while ( true )
    {
        m_pInputSystem->Update();
        m_pPhysicManager->Update();
        m_pUpdateManager->Update();
        m_pSceneManager->Update();
        m_pRenderManager->Render();
        m_pWindow->Run( m_pRenderManager->m_pGraphics->GetRender() );
    }
}



void GCGameManager::SetActiveGameManager()
{ GC::m_pActiveGameManager = this; }