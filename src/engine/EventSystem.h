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
	GCEventManager() = default;
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
	/// Registers a new event listener for a specific event type.
	/// </summary>
	/// <param name="type">The type of event to listen for</param>
	/// <param name="listener">The callback function to be called when the event occurs</param>
	/// <returns>A unique ListenerID that can be used to reference and manage the listener</returns>
	template<typename T>
	void Subscribe(GCEventType eventType, T* instance, GCEventCallback callback)
	{
		const GCEventCallback& func = callback;
		auto listener = m_eventListeners[eventType];
		if (listener.empty())
		{
			listener = std::vector<std::function<void(GCEvent&)>>();
		}
		listener.emplace_back(func);
	}

	/// <summary>
	/// Removes an event listener based on its type and unique ListenerID.
	/// </summary>
	/// <param name="type">The event type</param>
	/// <param name="id">The unique identifier ID to the callback</param>
	void RemoveEventListener();

private:
	/// <summary>
	/// Dispatches the event to all registered listeners for the event's type.
	/// The method is called internally to process events and call the appropriate listeners.
	/// </summary>
	/// <param name="e">Reference to the GCEvent object to be dispatched</param>
	void OnEvent(GCEvent& e);

private:
	GCMap<GCEventType, std::vector<std::function<void(GCEvent&)>>> m_eventListeners;
    GCQueue<GCEvent*> m_eventQueue;
};

class GCIEventListener {
public:
	/// <summary>
	/// Create a virtual
	/// </summary>
	virtual void OnEvent(GCEvent& ev) = 0;
};