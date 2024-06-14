#pragma once
#include <functional>
#include "Event.h"
#include "Layer.h"
#include "../core/Map.h"
#include "../core/Vector.h"
#include "../core/Queue.h"

using GCListener = std::function<void(GCEvent&)>;

class GCEventDispatcher 
{
public:
	GCEventDispatcher(GCEvent& gcevent) : m_gcEvent(gcevent) {}

	template<typename Type, typename Func>
	bool Dispatch(const Func& func) {
		if (m_gcEvent.GetEventType() == Type::GetStaticType())
		{
			m_gcEvent.IsHandle() = func(static_cast<Type&>(m_gcEvent));
			return true;
		}
		return false;
	}

private:
	GCEvent& m_gcEvent;
};

class GCEventSystem
{
public:
	/// <summary>
	/// Polls events from the operating system or framework 
	/// and dispatches them to the appropriate handlers.
	/// </summary>
	void PollEvents();

	/// <summary>
	/// Registers a new event listener for a specific event type.
	/// </summary>
	/// <param name="type">The type of event to listen for</param>
	/// <param name="listener">The callback function to be called when the event occurs</param>
	/// <returns>A unique ListenerID that can be used to reference and manage the listener</returns>
	void AddEventListener(GCListener);

	/// <summary>
	/// Removes an event listener based on its type and unique ListenerID.
	/// </summary>
	/// <param name="type">The event type</param>
	/// <param name="id">The unique identifier ID to the callback</param>
	void RemoveEventListener(GCListener);

	/// <summary>
	/// Adds a new layer to the event system.
	/// Layers can be used to manage different contexts in the application.
	/// </summary>
	/// <param name="layer">Pointer to the Layer object</param>
	void AddLayer(Layer* layer);

	/// <summary>
	/// Removes a layer from the event system.
	/// </summary>
	/// <param name="layer">Pointer to the Layer object to be removed</param>
	void RemoveLayer(Layer* layer);

private:
	/// <summary>
	/// Dispatches the event to all registered listeners for the event's type.
	/// The method is called internally to process events and call the appropriate listeners.
	/// </summary>
	/// <param name="e">Reference to the GCEvent object to be dispatched</param>
	void OnEvent(GCEvent& e);

private:
	GCMap<GCEventType, GCListener> m_eventListeners;
    GCQueue<GCEvent*> m_eventQueue;

	GCVector<Layer*> m_layers;
};