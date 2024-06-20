#include "GCWindow.h"

GCWindow::GCWindow(const GCWindowProperties& properties)
    : GCWindowBase(properties)
{
    m_properties = properties;
    m_window = nullptr;
    m_windowCallback = nullptr;
}

GCWindow::~GCWindow()
{
}

void GCWindow::SetWindowCallBack(const GCEventCallback& callback)
{
}

void GCWindow::OnUpdate()
{
}
