#include "pch.h"
#include "GCEngine.h"
#include "../core/Log.h"

GCEngine::GCEngine()
{
    //TODO: maybe use an encapsulated method to initialize the window
    m_window = new GCWindow(GCWindowProperties(L"GC Engine", 1600, 800));
    m_window->SetWindowCallBack(GC_BIND_EVENT_FN(GCEngine::OnEvent));
}

GCEngine::~GCEngine()
{
    delete m_window;
}

void GCEngine::Shutdown()
{
    Cleanup();
    m_isRunning = false;
}

void GCEngine::Run()
{
    if (!InitWindow())
    {
        return;
    }

    m_isRunning = true;

    while (m_isRunning)
    {
        m_window->PollEvents();
        m_window->OnUpdate();
    }

}

void GCEngine::OnEvent(GCEvent& ev)
{
    std::cout << ev.ToString() << std::endl;
}

bool GCEngine::InitWindow()
{
    if (!m_window->Initialize([this](){ Shutdown(); }))
    {
        return false;
    }
    return true;
}

bool GCEngine::InitD3D12()
{
    return false;
}

void GCEngine::Cleanup()
{
}

