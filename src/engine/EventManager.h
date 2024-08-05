#pragma once
#include "pch.h"



using GCEventCallback = std::function<void(GCEvent& ev)>;

struct EventHandler
{
	EventHandler(std::function<void()> cb)
	{
		count++;
        callback = cb;
	}

	EventHandler(const EventHandler& other)
	{
		count++;
	}

	EventHandler(EventHandler&& other) noexcept
	{
		count++;
	}

	EventHandler& operator=(const EventHandler& other)
	{
		if (this != &other)
		{
			count++;
		}
		return *this;
	}

	EventHandler& operator=(EventHandler&& other) noexcept
	{
		if (this != &other)
		{
			count++;
		}
		return *this;
	}
    static int GetCount() { return count; }

	void AddCallback(std::function<void()> callback)
	{
		this->callback = callback;
	}

    inline static int count = 0;
    std::function<void()> callback = nullptr;
};

class GCEventManager
{
public:
	GCEventManager();

	void Update();
	/// <summary>
	/// Polls events from the operating system or framework 
	/// and dispatches them to the appropriate handlers.
	/// </summary>
	void PollEvents();

	/// <summary>
    /// Push the created event to the event queue.
	/// </summary>
	/// <param name="ev">The pointer to the created event</param>
	void QueueEvent(GCEvent* ev);



	/// <summary>
	/// Registers a callback to event manager
	/// </summary>
	/// <param name="type">The type of event</param>
	/// <param name="callback">The callback function to be called when the event occurs</param>
	void Subscribe(GCEventType eventType, const GCEventCallback& callback)
	{
		const GCEventCallback& func = callback;
		m_eventCallbacks[eventType].push_back([=](GCEvent& ev) { func(ev); });
	}

	template<typename Event>
	void Subscribe(GCEventType eventType, void (*function)(Event&)) 
	{
		auto callback = [function](GCEvent& ev) {
			Event& event = static_cast<Event&>(ev);
			function(event);
			};
		m_eventCallbacks[eventType].push_back(callback);
	}

	/// <summary>
	/// Registers a callback based on event type,
	/// Callback does not need any ar
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <typeparam name="Event"></typeparam>
	/// <param name="eventType"></param>
	/// <param name="instance"></param>
	/// <param name="memberFunction"></param>
	template<typename T, typename Event>
	void Subscribe(GCEventType eventType, T* instance, void (T::* memberFunction)(Event&))
	{
		auto callback = [instance, memberFunction](GCEvent& ev)
			{
				Event& event = static_cast<Event&>(ev);
				(instance->*memberFunction)(event);
			};
		m_eventCallbacks[eventType].push_back(callback);
	}

	/// <summary>
	/// Removes an event listener based on its type and unique ListenerID.
	/// </summary>
	/// <param name="type">The event type</param>
	/// <param name="id">The unique identifier ID to the callback</param>
    void Unsubscribe(GCEventType type);

    void CallHandler(int handlerID);

    int AddHandler(EventHandler* handler)
    {
        m_eventHandlers.push_back(handler);
        return handler->GetCount();
    }

private:
	/// <summary>
	/// Dispatches the event to all registered callback for the event's type.
	/// The method is called internally to process events and call the appropriate callback.
	/// </summary>
	/// <param name="e">Reference to the GCEvent object to be dispatched</param>
	void OnEvent(GCEvent& e);

	void Exec(int handlerID);

private:
	std::map<GCEventType, std::vector<std::function<void(GCEvent&)>>> m_eventCallbacks;
    std::vector<EventHandler*> m_eventHandlers;
    GCQueue<GCEvent*> m_eventQueue;
};
