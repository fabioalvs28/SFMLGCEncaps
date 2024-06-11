#pragma once
#include <functional>
#include "Event.h"
#include "Layer.h"

using GCListenerID = size_t;

class GCEventDispatcher {
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
	void PollEvents();
	GCListenerID AddEventListener(GCEventType type, std::function<void(GCEvent&)> listener);
	void RemoveEventListener(GCEventType type, GCListenerID id);

	void AddLayer(Layer* layer);
	void RemoveLayer(Layer* layer);

	void OnEvent(GCEvent& e);

private:
	GCListenerID m_nextListenerID = 0;

	std::unordered_map<GCEventType, std::vector<std::pair<GCListenerID, std::function<void(GCEvent&)>>>> m_eventListeners;
    std::vector<Layer*> m_layers;
    std::vector<GCEvent> m_eventListenerID;
};