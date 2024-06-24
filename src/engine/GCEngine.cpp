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
    GCEventDispatcher dispatcher(ev);
    dispatcher.Dispatch<GCWindowResizeEvent>([this](GCWindowResizeEvent& e)
        {
            std::cout << "Window Resize Event: {0}" << e.GetName() 
                << " Width: {0}" << e.GetWidth() 
                << " Height: {0}" << e.GetHeight() 
                << std::endl;
            m_window->Resize(e.GetWidth(), e.GetHeight());
            return true;
        });

    dispatcher.Dispatch<GCMouseMoveEvent>([](GCMouseMoveEvent& e)
        {
            std::cout << "Mouse Move Event: {0}" << e.GetName() 
                << " X: {0}" << e.GetX() 
                << " Y: {0}" << e.GetY() 
                << std::endl;
            return true;
        });
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
    m_window->DestroyWindow();
}

