#include "pch.h"
#include "GameManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    //m_pEventManager = GCEventManager();
    //m_pSceneManager = GCSceneManager();
    //m_pInputManager = GCInputManager();
    //m_pPhysicManager = GCPhysicManager();
    m_pRenderManager = GCRenderManager();
}

void GCGameManager::Update()
{
    m_pRenderManager.Update();
    //m_pPhysicManager.Update();
    //m_pSceneManager.Update();
    //m_pSceneManager.NewDelete();
    //m_pSceneManager.Render();
}