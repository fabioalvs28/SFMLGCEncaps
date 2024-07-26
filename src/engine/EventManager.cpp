#include "pch.h"

GCEventManager::GCEventManager()
{
    for (int i = 0; i < (int)GCEventType::Count; i++)
    {
        m_eventCallbacks[(GCEventType)i] = std::vector<std::function<void(GCEvent&)>>();
    }
}

void GCEventManager::PollEvents()
{
    for (int i = 0; i < m_eventQueue.GetSize(); i++)
    {
        GCEvent* ev = m_eventQueue.Front();
        OnEvent(*ev);
        delete ev;
        m_eventQueue.Pop();
    }
}

void GCEventManager::QueueEvent(GCEvent* ev)
{
    m_eventQueue.Push(ev);
}

void GCEventManager::Unsubscribe(GCEventType type)
{

}

void GCEventManager::OnEvent(GCEvent& e)
{
    auto listeners = m_eventCallbacks[e.GetEventType()];
    for (auto& listener : listeners)
    {
        listener(e);
    }
}
