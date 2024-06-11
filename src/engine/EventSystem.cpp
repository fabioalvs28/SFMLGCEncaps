#include "pch.h"
#include "EventSystem.h"

void GCEventSystem::PollEvents()
{
}

GCListenerID GCEventSystem::AddEventListener(GCEventType type, std::function<void(GCEvent&)> listener)
{
    GCListenerID id = m_nextListenerID++;
    m_eventListeners[type].emplace_back(id, listener);
    return id;
}

void GCEventSystem::RemoveEventListener(GCEventType type, GCListenerID id)
{  
    auto it = m_eventListeners.find(type);
    if (it != m_eventListeners.end())
    {
        auto& listeners = it->second;
        auto listenerIt = std::remove_if(listeners.begin(), listeners.end(),
            [id](const std::pair<GCListenerID, std::function<void(GCEvent&)>>& element)
            {
                return element.first == id;
            });
        if (listenerIt != listeners.end())
        {
            listeners.erase(listenerIt, listeners.end());
        }
    }
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
            listener.second(e);
        }
    }
}
