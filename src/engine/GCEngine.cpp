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
}

void GCEngine::Run()
{
}

void GCEngine::OnEvent(GCEvent& ev)
{
}

bool GCEngine::InitWindow()
{
    return false;
}

bool GCEngine::InitD3D12()
{
    return false;
}

void GCEngine::Cleanup()
{
}

