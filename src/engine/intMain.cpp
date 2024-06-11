#include "pch.h"

#include <iostream>
#include "Window.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    GCEventSystem eventSystem;
    GCWindow window(hInstance, nCmdShow, eventSystem);

    if (!window.Create(L"Learn to Program Windows", 800, 600))
    {
        return 0;
    }

    window.Show(nCmdShow);

    // Register event listeners
    eventSystem.AddEventListener(GCEventType::WindowResize, [](GCEvent& e)
        {
            auto& resizeEvent = static_cast<GCWindowResizeEvent&>(e);
            std::wcout << L"Window resized to " << resizeEvent.GetWidth() << L"x" << resizeEvent.GetHeight() << std::endl;
        });

    eventSystem.AddEventListener(GCEventType::MouseButtonPressed, [](GCEvent& e)
        {
            auto& mouseEvent = static_cast<GCMouseButtonPressed&>(e);
            std::wcout << L"Mouse button pressed at (" << mouseEvent.GetX() << L", " << mouseEvent.GetY() << L")" << std::endl;
        });

    while (true)
    {
        window.PollEvents();
        // Run the rest of the game loop
    }

    return 0;
}
