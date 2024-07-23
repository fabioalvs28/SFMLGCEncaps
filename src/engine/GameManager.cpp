#include "pch.h"
#include "GameManager.h"

#include "GC.h"

// TODO InputManager Implementation
// TODO Timer Implementation

GCGameManager::GCGameManager()
{
    m_pNode = nullptr;
    m_pInputSystem = GCInputSystem();
    m_pPhysicManager = GCPhysicManager();
    m_pUpdateManager = GCUpdateManager();
    m_pEventManager = GCEventManager();
    m_pSceneManager = GCSceneManager();
    m_pRenderManager = GCRenderManager();
    m_Time.Reset();
}

void GCGameManager::Update()
{
	m_Time.Update();
    m_pInputSystem.Update();
    m_pPhysicManager.Update();
    m_pUpdateManager.Update();
    m_pSceneManager.Update();
    m_pRenderManager.Render();
}



void GCGameManager::SetActiveGameManager()
{
    GC::m_pActiveGameManager = this;
}