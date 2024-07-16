#include "pch.h"
#include "GameManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pRenderManager = GCRenderManager();
    m_pPhysicManager = GCPhysicManager();
    m_pUpdateManager = GCUpdateManager();
    m_pEventManager = GCEventManager();
    m_pSceneManager = GCSceneManager();
}

void GCGameManager::Update()
{
    m_pRenderManager.Render();
    m_pPhysicManager.Update();
    m_pUpdateManager.Update();
    m_pSceneManager.Update();
}