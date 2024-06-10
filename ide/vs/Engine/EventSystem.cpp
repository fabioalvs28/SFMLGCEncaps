#include "pch.h"
#include "EventSystem.h"

void GCEventSystem::PollEvents()
{
}

void GCEventSystem::AddEventListener(GCEventType type, std::function<void(GCEvent&)> listener)
{
    m_eventListeners[type].push_back(listener);
}

void GCEventSystem::RemoveEventListener(GCEventType type, std::function<void(GCEvent&)> listener)
{  
    //auto it = m_eventListeners.find(type);
    //if (it != m_eventListeners.end())
    //{
    //    auto& listeners = it->second;
    //    auto it2 = std::find(listeners.begin(), listeners.end(), listener);
    //    if (it2 != listeners.end())
    //    {
    //        listeners.erase(it2);
    //    }
    //}
}

void GCEventSystem::AddLayer(Layer* layer)
{
    m_layers.push_back(layer);
}

void GCEventSystem::RemoveLayer(Layer* layer)
{
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end())
    {
        m_layers.erase(it);
    }
}

void GCEventSystem::OnEvent(GCEvent& e)
{
    auto it = m_eventListeners.find(e.GetEventType());
    if (it != m_eventListeners.end())
    {
        for (auto& listener : it->second)
        {
            listener(e);
        }
    }
}
