#include "pch.h"
#include "GameManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pRenderManager = GCRenderManager();
    m_pEventManager = GCEventManager();
    m_pSceneManager = GCSceneManager();
    m_pPhysicManager = GCPhysicManager();
}

void GCGameManager::Update()
{
    m_pRenderManager.Render();
    //m_pInputManager.Update();
    //m_pPhysicManager.Update();
    //m_pSceneManager.Update();
    //m_pSceneManager.NewDelete();
    //m_pSceneManager.Render();
}