#pragma once
#include "pch.h"



using GCEventCallback = std::function<void(GCEvent& ev)>;

struct EventHandler
{
	EventHandler( std::function<void()> cb )
	{
		count++;
        callback = cb;
	}

	EventHandler( const EventHandler& other )
	{ count++; }

	EventHandler( EventHandler&& other ) noexcept
	{ count++; }

	EventHandler& operator=( const EventHandler& other )
	{
		if ( this != &other )
			count++;
		return *this;
	}

	EventHandler& operator=( EventHandler&& other ) noexcept
	{
		if ( this != &other )
			count++;
		return *this;
	}
    static int GetCount() { return count; }

	void AddCallback( std::function<void()> callback )
	{ this->callback = callback; }

    inline static int count = 0;
    std::function<void()> callback = nullptr;
};

class GCEventManager
{
public:
	GCEventManager();

	void Update();
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Polls events from the operating system or framework and dispatches them to the appropriate handlers.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PollEvents();
	
	////////////////////////////////////////////////////////
	/// @brief Push the created event to the event queue.
	/// 
	/// @param pEvent The pointer to the created event.
	////////////////////////////////////////////////////////
	void QueueEvent( GCEvent* pEvent );
	
	
	
	////////////////////////////////////////////////////////////////////////////////
	/// @brief Registers a callback to event manager.
	/// 
	/// @param eventType The type of event.
	/// @param callback The callback function to be called when the event occurs.
	////////////////////////////////////////////////////////////////////////////////
	void Subscribe( GCEventType eventType, const GCEventCallback& callback )
	{
		const GCEventCallback& func = callback;
		m_eventCallbacks[ eventType ].push_back( [=]( GCEvent& event ) { func( event ); } );
	}

	template<typename Event>
	void Subscribe( GCEventType eventType, void (*function)(Event&) ) 
	{
		auto callback = [ function ]( GCEvent& ev )
		{
			Event& event = static_cast<Event&>( ev );
			function( event );
		};
		m_eventCallbacks[eventType].push_back( callback );
	}

	///////////////////////////////////////////////////////
	/// @brief Registers a callback based on event type.
	/// 
	/// @tparam T 
	/// @tparam Event 
	/// 
	/// @param eventType 
	/// @param instance 
	/// @param memberFunction 
	/// 
	/// @note Callback does not need any arguments.
	///////////////////////////////////////////////////////
	template<typename T, typename Event>
	void Subscribe( GCEventType eventType, T* instance, void (T::* memberFunction)(Event&) )
	{
		auto callback = [ instance, memberFunction ]( GCEvent& ev )
			{
				Event& event = static_cast<Event&>( ev );
				( instance->*memberFunction )( event );
			};
		m_eventCallbacks[eventType].push_back( callback );
	}
    
	////////////////////////////////////////////////////////////////////////////////
	/// @brief Removes an event listener based on its type and unique ListenerID.
	/// 
	/// @param type The event type.
	////////////////////////////////////////////////////////////////////////////////
	void Unsubscribe( GCEventType type );

    void CallHandler( int handlerID );

    int AddHandler( EventHandler* handler )
    {
        m_eventHandlers.push_back( handler );
        return handler->GetCount();
    }

private:
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Dispatches the event to all registered callback for the event's type.
	/// 
	/// @param event Reference to the GCEvent object to be dispatched.
	/// 
	/// @note The method is called internally to process events and call the appropriate callback.
	/////////////////////////////////////////////////////////////////////////////////////////////////
	void OnEvent( GCEvent& event );

	void Exec( int handlerID );

private:
	std::map<GCEventType, std::vector<std::function<void(GCEvent&)>>> m_eventCallbacks;
    std::vector<EventHandler*> m_eventHandlers;
    GCQueue<GCEvent*> m_eventQueue;
};
