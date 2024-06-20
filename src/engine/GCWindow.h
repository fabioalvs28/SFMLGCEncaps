#pragma once

#include "Window.h"


class GCWindow : public GCWindowBase
{
public:
    GCWindow(const GCWindowProperties& properties);
    virtual ~GCWindow() = default;

    void SetWindowCallBack(const GCEventCallback&) override;
    void OnUpdate() override;

    void* GetNativeWindow() const { return m_window; }

private:
    void* m_window;
};