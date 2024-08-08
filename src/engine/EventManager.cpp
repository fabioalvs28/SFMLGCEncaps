#include "pch.h"

GCEventManager::GCEventManager()
{
    m_eventQueue = GCQueue<GCEvent*>();
    for ( int i = 0; i < (int)GCEventType::Count; i++ )
        m_eventCallbacks[ (GCEventType)i ] = std::vector<std::function<void(GCEvent&)>>();
}

void GCEventManager::Update()
{ PollEvents(); }

void GCEventManager::PollEvents()
{
    if ( m_eventQueue.IsEmpty() )
        return;

    for ( int i = 0; i < m_eventQueue.GetSize(); i++ )
    {
        GCEvent* pEvent = m_eventQueue.Front();
        OnEvent( *pEvent );
        delete pEvent;
        m_eventQueue.Pop();
    }
}

void GCEventManager::QueueEvent( GCEvent* pEvent )
{ m_eventQueue.Push( pEvent ); }

void GCEventManager::Unsubscribe( GCEventType type ) {}

void GCEventManager::CallHandler(int handlerID)
{
    Exec(handlerID);
}

void GCEventManager::OnEvent( GCEvent& event )
{
    auto listeners = m_eventCallbacks[ event.GetEventType() ];
    for ( auto& listener : listeners )
        listener( event );
}

void GCEventManager::Exec( int handlerID )
{
    for ( auto& handler : m_eventHandlers )
        if ( handler->GetCount() == handlerID )
            handler->callback();
}

