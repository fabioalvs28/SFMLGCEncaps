#include "pch.h"
#include "EventSystem.h"

//Call in main loop
void GCEventManager::PollEvents()
{
    for (int i = 0; i < m_eventQueue.GetSize(); i++)
    {
        //OnEvent(m_eventQueue.);
    }
}

void GCEventManager::PushEvent(GCEvent* ev)
{
    m_eventQueue.Push(ev);
}


void GCEventManager::RemoveEventListener()
{
}


void GCEventManager::OnEvent(GCEvent& e)
{
    auto listeners = m_eventListeners[e.GetEventType()];

}
