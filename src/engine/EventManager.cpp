#include "pch.h"
#include "EventManager.h"

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

void GCEventManager::PushEvent(GCEvent* ev)
{
    m_eventQueue.Push(ev);
}


void GCEventManager::Subscribe(int keyCode, BYTE state, std::function<void()> func)
{
    auto callback = [func](GCEvent&) { func(); };
    m_keyboardInputManager->SetKeyInputCallback(state, keyCode, callback);
}

void GCEventManager::Unsubscribe(GCEventType type)
{
}

void GCEventManager::SetKeyboardInputManager(GCKeyboardInputManager* keyboardInputManager)
{
    m_keyboardInputManager = keyboardInputManager;

}

void GCEventManager::OnEvent(GCEvent& e)
{
    auto listeners = m_eventCallbacks[e.GetEventType()];
    for (auto& listener : listeners)
    {
        listener(e);
    }

    //TODO: Refactor later, it needs to be handle seperately
    for (auto& callback : m_systemCallback)
    {
        callback(e);
    }

}
