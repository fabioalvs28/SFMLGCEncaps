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

void GCEventSystem::PushEvent(GCEvent* ev)
{
    m_eventQueue.Push(ev);
}


void GCEventSystem::AddEventListener(const GCEvent& ev, std::function<void()> func)
{
    m_eventListeners[ev.GetEventType()].push_back(func);
}

void GCEventSystem::RemoveEventListener()
{ 
    //Check if the event is handled by the listener
}


void GCEventSystem::OnEvent(GCEvent& e)
{
    GCEventDispatcher dispatcher(e);
    std::vector<std::function<void()>> listeners = m_eventListeners.GetValue(e.GetEventType());
    for (int i = 0; i < listeners.size(); i++)
    {
        listeners[i]();
    }
}
