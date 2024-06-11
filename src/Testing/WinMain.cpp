#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "ECSTesting.h"

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
    MessageBox(NULL, L"(Hello World!)", L"(Hello)", MB_OK);
    FreeConsole();
}