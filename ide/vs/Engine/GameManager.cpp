#include "pch.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "InputManager.h"

void GCGameManager::Init()
{
    m_pSceneManager = new GCSceneManager();
    m_pInputManager = new GCInputManager();
}

void GCGameManager::Update()
{
    m_pSceneManager->Update();
}