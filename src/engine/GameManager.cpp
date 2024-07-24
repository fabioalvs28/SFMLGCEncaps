#include "pch.h"
#include "GameManager.h"

#include "../Render/Window.h"
#include "GC.h"

// TODO Timer Implementation

GCGameManager::GCGameManager( HINSTANCE hInstance )
{
    m_pNode = nullptr;
    m_pWindow = new Window( hInstance );
    m_pWindow->Initialize();
    
    m_inputSystem = GCInputSystem();
    m_eventManager = GCEventManager();
    m_physicManager = GCPhysicManager();
    m_updateManager = GCUpdateManager();
    m_sceneManager = GCSceneManager();
    m_renderManager = GCRenderManager();
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
        m_inputSystem.Update();
        m_physicManager.Update();
        m_updateManager.Update();
        m_sceneManager.Update();
        m_renderManager.Render();
        m_pWindow->Run( m_renderManager.m_pGraphics->GetRender() );
    }
}



void GCGameManager::SetActiveGameManager()
{ GC::m_pActiveGameManager = this; }