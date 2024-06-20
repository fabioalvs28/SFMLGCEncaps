#include "GCEngine.h"

GCEngine::GCEngine()
{
    m_window = new GCWindow(GCWindowProperties(L"GC Engine", 1600, 800));
    m_window->SetWindowCallBack(std::bind(&GCEngine::OnEvent, this, std::placeholders::_1));
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

