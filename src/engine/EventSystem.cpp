#include "pch.h"
#include "EventSystem.h"

void GCEventSystem::PollEvents()
{
}

GCListenerID GCEventSystem::AddEventListener(GCListener)
{
    GCListenerID id = m_nextListenerID++;

    return id;
}

void GCEventSystem::RemoveEventListener(GCListenerID id)
{  

}

void GCEventSystem::AddLayer(Layer* layer)
{
    /*m_layers.push_back(layer);*/
}

void GCEventSystem::RemoveLayer(Layer* layer)
{
    //auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    //if (it != m_layers.end())
    //{
    //    m_layers.erase(it);
    //    delete layer;
    //}
}

void GCEventSystem::OnEvent(GCEvent& e)
{

}
