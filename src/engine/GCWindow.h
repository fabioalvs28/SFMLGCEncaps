#pragma once

#include "Window.h"

typedef std::function<void()> WindowCloseCallback;

class GCWindow : public GCWindowBase
{
public:
    GCWindow(const GCWindowProperties& properties);
    virtual ~GCWindow() = default;

    void SetWindowCallBack(const GCEventCallback& callback) override { m_windowCallback = callback; }
    void OnUpdate() override;
    void PollEvents();

    bool Initialize(WindowCloseCallback closeCallback);
    void DestroyWindow();
private:
    static LRESULT WINAPI OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hWnd = nullptr;
    WindowCloseCallback m_windowCloseCallback = nullptr;
    GCEventCallback m_windowCallback = nullptr;
};