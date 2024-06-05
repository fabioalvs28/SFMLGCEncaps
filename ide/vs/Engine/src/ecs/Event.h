#pragma once
#include <string>

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

/// <summary>
/// Base event type class
/// Derived class should have a static get method to work
/// </summary>
class GCEvent {
public:
	virtual GCEventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
	virtual ~GCEvent() = default;

protected:
	bool m_isHandle = false;
};