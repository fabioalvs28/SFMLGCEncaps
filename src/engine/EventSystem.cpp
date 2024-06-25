#include "pch.h"
#include "EventSystem.h"

//Call in main loop
void GCEventSystem::PollEvents()
{
    for (int i = 0; i < m_eventQueue.GetSize(); i++)
    {
        
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
}


void GCEventSystem::OnEvent(GCEvent& e)
{
    GCEventDispatcher dispatcher(e);

    dispatcher.Dispatch<GCKeyPressedEvent>([](GCKeyPressedEvent& ev)
        {
            //OnKeyPressed();
            return true;
        });

    dispatcher.Dispatch<GCKeyReleased>([](GCKeyReleased& ev)
        {
            //OnKeyReleased();
            return true;
        });
}
