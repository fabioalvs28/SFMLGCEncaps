#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "ECSTesting.h"
#include "Window.h"
#include "EventSystem.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "GCColor.h"
#include "Log.h"


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

	

	LogEngineDebug("Engine Debug");
    LogEngineError("Engine Error");
	LogEngineWarn("Engine Warn");
    LogEngineInfo("Engine Info");
	LogEngineTrace("Engine Trace");

	LogGameDebug("Game Debug");

    while (w->IsRunning())
    {
        w->PollEvents();
    }

    
    FreeConsole();
}