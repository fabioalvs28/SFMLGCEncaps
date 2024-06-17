#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "ECSTesting.h"
#include "../engine/Window.h"
#include "../engine/EventSystem.h"
#include "../Engine/GameManager.h"
#include "../Engine/SceneManager.h"
#include "../Engine/InputManager.h"
#include "../Engine/Scene.h"
#include "../Engine/GameObject.h"
#include "../Engine/Components.h"
#include "../Engine/GCColor.h"


void CreateConsole()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CreateConsole();
    ECSTesting::GetInstance().RunTests();
    std::cout << "Logging to console..." << std::endl;
    
    GCEventSystem* ev = new GCEventSystem();
    GCWindow *w = new GCWindow(hInstance, nCmdShow, *ev);
    w->Create(L"Testing", 800, 600);
    w->Show(nCmdShow);

	GCGameManager *GameManager = new GCGameManager();
    GameManager->Init();

	GCScene *myScene = GCSceneManager::CreateScene();
	myScene->SetActive(true);

    GCGameObject *myGoOne = myScene->CreateGameObject("go", true);
	GCGameObject *myGoTwo = myScene->CreateGameObject("go2", true);

	myGoOne->AddComponent<SpriteRenderer>();
	myGoTwo->AddComponent<SpriteRenderer>();

	GCColor color = GCColor(1.0f, 1.0f, 1.0f, 1.0f);
	myGoOne->GetComponent<SpriteRenderer>()->SetColor(color);
	myGoTwo->RemoveComponent<SpriteRenderer>();
	myGoOne->AddComponent<BoxCollider>();
    myGoOne->Destroy();

    while (w->IsRunning())
    {
        
		myGoTwo->m_transform.m_position.x += 0.01f;

		std::cout << "Position  " << myGoTwo->m_transform.m_position.x << std::endl;


	    GameManager->Update();
        w->PollEvents();
    }

    
    FreeConsole();
}