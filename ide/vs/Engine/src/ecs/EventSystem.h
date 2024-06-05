#pragma once
#include <functional>
#include "Event.h"

class GCEventDispatcher {
public:
	GCEventDispatcher(GCEvent& gcevent) : m_gcEvent(gcevent) {}

	template<typename Type, typename Func>
	bool Dispatch(const Func& func) {
		if (m_gcEvent.GetEventType() == Type::GetStaticType())
		{
			m_gcEvent.m_isHandle = func(static_cast<Type&>(m_gcEvent));
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
	void PollEvents();
	void AddEventListener();
	void RemoveEventListener();

private:
	void OnEvent(GCEvent& e);

	std::unordered_map<GCEventType, std::vector<std::function<void(GCEvent&)>>> m_eventListeners; //Maybe later to use the custom hashmap
};