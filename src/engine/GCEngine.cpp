#include "pch.h"
#include "GCEngine.h"
#include "../core/Log.h"
#include "GC.h"

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
    GCEventManager eventmanager = GCEventManager();
    //GCInputManager inputmanger(&eventmanager);

    if (!InitWindow())
    {
        return;
    }

    if (!InitEngine())
    {
        return;
    }

    m_isRunning = true;

    while (m_isRunning)
    {
        //inputmanger.UpdateInputs();
        eventmanager.PollEvents();
        m_window->PollEvents();
        m_window->OnUpdate();
    }

}

void GCEngine::OnEvent(GCEvent& ev)
{
    GCEventDispatcher dispatcher(ev);

    dispatcher.Dispatch<GCWindowCloseEvent>([this](GCWindowCloseEvent& e)
        {
            std::cout << "Window Close Event: " << e.GetName() << std::endl;
            Shutdown();
            return true;
        });

    dispatcher.Dispatch<GCWindowResizeEvent>([this](GCWindowResizeEvent& e)
        {
            std::cout << "Window Resize Event: " << e.GetName() 
                << " Width: " << e.GetWidth() 
                << " Height: " << e.GetHeight() 
                << std::endl;
            m_window->Resize(e.GetWidth(), e.GetHeight());
            return true;
        });

    dispatcher.Dispatch<GCMouseMoveEvent>([](GCMouseMoveEvent& e)
        {
            std::cout << "Mouse Move Event: " << e.GetName() 
                << " X: " << e.GetX() 
                << " Y: " << e.GetY() 
                << std::endl;
            return true;
        });

    dispatcher.Dispatch<GCMouseButtonPressed>([](GCMouseButtonPressed& e)
        {
            std::cout << "Mouse Button Event: " << e.GetName()
                << " Button: Left" 
                << std::endl;
            return true;
        });
}

bool GCEngine::InitWindow()
{
    if (!m_window->Initialize())
    {
        return false;
    }
    return true;
}

bool GCEngine::InitEngine()
{
    //TODO: Initialize the engine components

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

