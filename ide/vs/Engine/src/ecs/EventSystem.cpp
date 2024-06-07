#include "EventSystem.h"

void GCEventSystem::PollEvents()
{
}

void GCEventSystem::AddEventListener()
{
}

void GCEventSystem::RemoveEventListener()
{
}

void GCEventSystem::OnEvent(GCEvent& e)
{
    //TODO: Dispatch different event to listeners
    auto it = m_eventListeners.find(e.GetEventType());
    if (it != m_eventListeners.end())
    {
        for (auto& listener : it->second)
        {
            listener(e);
        }
    }
}
