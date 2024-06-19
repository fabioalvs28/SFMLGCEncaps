#pragma once
#include <Windows.h>
#include "EventSystem.h"

class GCWindow
{
public:
    GCWindow(HINSTANCE hInstance, int nCmdShow, GCEventSystem& eventSystem);
    ~GCWindow();

    bool Create(LPCWSTR windowName, int width, int height);
    void Show(int nCmdShow);
    void PollEvents();
	bool IsRunning() { return m_isRunning; }

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void Close() { m_isRunning = false; }
	void EnableVirtualTerminalProcessing();

private:
    HINSTANCE m_hInstance;
    HWND m_hwnd;
    GCEventSystem& m_eventSystem;
    LPCWSTR m_className = L"SampleWindowClass";
	bool m_isRunning = true;
};
