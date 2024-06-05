#pragma once
#include <functional>
#include "Event.h"

class GCEventDispatcher {
    using Callback = std::function<void()>;

    template<typename ...Args>
    void AddListener(GCEvent<Args...>& event, std::function<void(Args ...)> listener) { event += listener; }

    template<typename... Args>
    void RemoveListener(GCEvent<Args...>& event, std::function<void(Args...)> listener) { event -= listener; }

    template<typename ...Args>
    void Dispatch(GCEvent<Args...>& event, Args... args) { event.Invoke(args...); }
};
