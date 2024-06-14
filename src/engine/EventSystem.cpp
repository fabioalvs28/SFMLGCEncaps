#include "pch.h"
#include "EventSystem.h"

void GCEventSystem::PollEvents()
{
    while (!m_eventQueue.IsEmpty()) 
    {
        GCEvent* ev = m_eventQueue.Front();
        m_eventQueue.Pop();

        OnEvent(*ev);
        delete ev;
    }
}

void GCEventSystem::AddEventListener(GCListener)
{
}

void GCEventSystem::RemoveEventListener(GCListener)
{ 
}

void GCEventSystem::AddLayer(Layer* layer)
{
}

void GCEventSystem::RemoveLayer(Layer* layer)
{
}

void GCEventSystem::OnEvent(GCEvent& e)
{
    auto& listener = m_eventListeners.GetValue(e.GetEventType());
    listener(e);
}
