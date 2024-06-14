#include "pch.h"
#include "EventSystem.h"

GCEventSystem::GCEventSystem()
{
    //For test, remove later
    m_eventListeners.Insert(GCEventType::MouseButtonPressed, new GCVector<std::function<void()>>);
}

void GCEventSystem::PollEvents(MSG msg)
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
    m_eventListeners[ev.GetEventType()]->PushBack(func);
}

void GCEventSystem::RemoveEventListener()
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
    GCVector<std::function<void()>>* listeners = m_eventListeners.GetValue(e.GetEventType());
    for (int i = 0; i < listeners->GetSize(); i++)
    {
        listeners[i];
    }
}
