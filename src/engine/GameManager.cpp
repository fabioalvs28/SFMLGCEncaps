#include "pch.h"
#include "GameManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pSceneManager = GCSceneManager();
    // m_pInputManager = GCInputManager();
}

void GCGameManager::Update()
{
    m_pSceneManager.Update();
    m_pSceneManager.NewDelete();
    m_pSceneManager.Render();
}