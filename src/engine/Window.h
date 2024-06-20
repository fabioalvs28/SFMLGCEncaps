#pragma once
#include <Windows.h>
#include "Event.h"

struct GCWindowProperties
{
    const wchar_t* Title;
    int32_t Width = 0, Height = 0;

    GCWindowProperties(const wchar_t* title = L"GC Engine", int32_t width = 1600, int32_t height = 800)
        : Title(title), Width(width), Height(height)
    {
    }
};

//macro to bind the event function
//Reminders: if use in a class, use GC_BIND_EVENT_FN(ClassName::FunctionName)
#define GC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

using GCEventCallback = std::function<void(GCEvent& ev)>;

class GCWindowBase
{
public:
    virtual ~GCWindowBase() = default;

    virtual void SetWindowCallBack(const GCEventCallback&) = 0;

    virtual void OnUpdate() = 0;

    int32_t GetWidth() const { return m_properties.Width; }
    int32_t GetHeight() const { return m_properties.Height; }

private:
	void EnableVirtualTerminalProcessing(); //TODO: Move it to Testing.proj

protected:
    GCWindowProperties m_properties;
    GCEventCallback m_windowCallback = nullptr;
};
