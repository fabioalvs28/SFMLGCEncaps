#pragma once
#include <functional>
#include <vector>

enum class GCEventType
{
	WindowClose,
	WindowResize,
	KeyPressed,
	KeyReleased,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMove,
	MouseScrolled,
};

class GCEvent {
public:
	virtual GCEventType GetEventType() const = 0;
};