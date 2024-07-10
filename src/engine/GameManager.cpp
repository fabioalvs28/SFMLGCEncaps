#include "pch.h"
#include "GameManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pPhysicManager = GCPhysicManager();
    m_pUpdateManager = GCUpdateManager();
    m_pEventManager = GCEventManager();
    m_pSceneManager = GCSceneManager();
}

void GCGameManager::Update()
{
    m_pPhysicManager.Update();
    m_pUpdateManager.Update();
    m_pSceneManager.Update();
    m_pSceneManager.NewDelete();
    m_pSceneManager.Render();
}