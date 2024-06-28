#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "Window.h"
#include "EventSystem.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "GCColor.h"
#include "Log.h"
#include "GCEngine.h"

void CreateDebugConsole()
{
    AllocConsole();

    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);

    SetConsoleTitle(L"Debug Console");

    std::cin.clear();
    std::cout.clear();
    std::cerr.clear();
    std::wcin.clear();
    std::wcout.clear();
    std::wcerr.clear();

    std::ios::sync_with_stdio();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    EnableMemoryLeakDetection();
    CreateDebugConsole();

    GCEngine::Get().Run();

    FreeConsole();
    return 0;
}