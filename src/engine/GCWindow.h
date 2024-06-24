#pragma once

#include "Window.h"

typedef std::function<void()> WindowCallback;

class GCWindow : public GCWindowBase
{
public:
    GCWindow(const GCWindowProperties& properties);
    virtual ~GCWindow() = default;

    void SetWindowCallBack(const GCEventCallback& callback) override { m_windowCallback = callback; }
    void OnUpdate() override;
    void PollEvents();

    bool Initialize();
    void DestroyWindow();

    void* GetNativeWindow() override { return m_hWnd; }

private:
    static LRESULT WINAPI OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hWnd = nullptr;
    GCEventCallback m_windowCallback = nullptr;
};
