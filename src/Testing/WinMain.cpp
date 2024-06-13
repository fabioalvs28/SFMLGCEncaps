#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "ECSTesting.h"
#include "../engine/Window.h"
#include "../engine/EventSystem.h"

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
    GCWindow w = GCWindow(hInstance, nCmdShow, *ev);
    w.Create(L"Testing", 800, 600);
    w.Show(nCmdShow);

    w.PollEvents();
    
    FreeConsole();
}