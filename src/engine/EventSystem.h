#pragma once
#include <functional>

#include "Event.h"
#include "Layer.h"
#include "../core/Map.h"
#include "../core/Vector.h"
#include "../core/Queue.h"

using GCEventCallback = std::function<void(GCEvent& ev)>;

class GCEventManager
{
public:
	GCEventManager();
	/// <summary>
	/// Polls events from the operating system or framework 
	/// and dispatches them to the appropriate handlers.
	/// </summary>
	void PollEvents();

	/// <summary>
    /// Push the created event to the event queue.
	/// </summary>
	/// <param name="ev">The pointer to the created event</param>
	void PushEvent(GCEvent* ev);

	/// <summary>
	/// Registers a callback to event manager
	/// </summary>
	/// <param name="type">The type of event</param>
	/// <param name="callback">The callback function to be called when the event occurs</param>
	template<typename T>
	void Subscribe(GCEventType eventType, T* instance, const GCEventCallback& callback)
	{
		const GCEventCallback& func = callback;
		m_eventCallback[eventType].push_back([=](GCEvent& ev) { func(ev); });
	}

	/// <summary>
	/// Subscribe a function without any event type 
	/// but the function should have a GCEvent as parameter 
	/// </summary>
	/// <param name="instance">the pointer to class itself</param>
	/// <param name="memberFunction">function to be bind</param>
	template<typename T>
	void Subscribe(T* instance, void(T::*memberFunction)(GCEvent&))
	{
		auto func = std::bind(memberFunction, instance, std::placeholders::_1);
		m_systemCallback.push_back(func);
	}

	/// <summary>
	/// Removes an event listener based on its type and unique ListenerID.
	/// </summary>
	/// <param name="type">The event type</param>
	/// <param name="id">The unique identifier ID to the callback</param>
    void Unsubscribe(GCEventType type /*pass string as second parameter to identify the listener*/);

private:
	/// <summary>
	/// Dispatches the event to all registered callback for the event's type.
	/// The method is called internally to process events and call the appropriate callback.
	/// </summary>
	/// <param name="e">Reference to the GCEvent object to be dispatched</param>
	void OnEvent(GCEvent& e);

private:
	std::map<GCEventType, std::vector<std::function<void(GCEvent&)>>> m_eventCallback;
	std::vector<std::function<void(GCEvent&)>> m_systemCallback;
    GCQueue<GCEvent*> m_eventQueue;
};
