#include "pch.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "InputManager.h"

// TODO InputManager Implementation
// TODO Timer Implementation

void GCGameManager::Init()
{
    m_pSceneManager = new GCSceneManager();
    m_pInputManager = new GCInputManager();
}

void GCGameManager::Update()
{
    GCSceneManager::Update();
    GCSceneManager::NewDelete();
    GCSceneManager::Render();
}