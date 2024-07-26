#include "pch.h"
#include "GameManager.h"

#include "../Render/pch.h"

///////////////////////////////////////////////////////////////////
/// @brief Default constructor for the GameManager class.
/// 
/// @param hInstance A pointer to the windows's app's hInstance.
///////////////////////////////////////////////////////////////////
GCGameManager::GCGameManager( HINSTANCE hInstance )
{
    m_pNode = nullptr;
    m_pWindow = new Window( hInstance );
    m_pWindow->Initialize();
    
    m_pTimer = new GCTime();
    m_pInputSystem = new GCInputSystem();
    m_pEventManager = new GCEventManager();
    m_pPhysicManager = new GCPhysicManager();
    m_pUpdateManager = new GCUpdateManager();
    m_pSceneManager = new GCSceneManager();
    m_pRenderManager = new GCRenderManager( m_pWindow );
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Starts this GameManager's GameLoop.
/// 
/// @note If there was already an active GameManager, its GameLoop will be stopped by this function.
///////////////////////////////////////////////////////////////////////////////////////////////////////
void GCGameManager::Run()
{
    //! Stop the GameLoop from the old active GameManager if there is one
    SetActiveGameManager();
    GameLoop();
}

////////////////////////////
/// @brief Runs the Game.
////////////////////////////
void GCGameManager::GameLoop()
{
    while ( true )
    {
        m_pTimer->Update();
        m_pInputSystem->Update();
        m_pPhysicManager->Update();
        m_pUpdateManager->Update();
        m_pSceneManager->Update();
        m_pRenderManager->Render();
        m_pWindow->Run( m_pRenderManager->m_pGraphics->GetRender() );
    }
}



//////////////////////////////////////////////////////////////
/// @brief Sets this GameManager as the active GameManager.
//////////////////////////////////////////////////////////////
void GCGameManager::SetActiveGameManager()
{ GC::m_pActiveGameManager = this; }