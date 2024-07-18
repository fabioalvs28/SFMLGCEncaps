#include "pch.h"
#include "GameManager.h"

#include "GC.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pNode = nullptr;
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



void GCGameManager::SetActiveGameManager()
{ GC::m_pActiveGameManager = this; }