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

enum class GCMouseButton
{
	Left,
	Right,
	Middle
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

#pragma region MouseEvent
class GCMouseButtonPressed : public GCEvent
{
public:
	GCMouseButtonPressed(int x, int y, GCMouseButton mouseButton)
		: m_x(x), m_y(y), m_mouseButton(mouseButton) {}

	static GCEventType GetStaticType() { return GCEventType::MouseButtonPressed; }
	GCEventType GetEventType() const override { return GetStaticType(); }

	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
	GCMouseButton GetMouseButton() const { return m_mouseButton; }

private:
	int m_x, m_y;
	GCMouseButton m_mouseButton;
};

#pragma endregion

#pragma region WindowEvents
class GCWindowCloseEvent : public GCEvent {
public:
	GCWindowCloseEvent() {}

	GCEventType GetEventType() const override { return GCEventType::WindowClose; }
	const char* GetName() const override { return "WindowCloseEvent"; }
};

/// <summary>
/// Event occurs when application's window is resized
/// </summary>
class GCWindowResizeEvent : public GCEvent {
public:
	GCWindowResizeEvent(unsigned int width, unsigned int height) 
		: m_width(width), m_height(height) {}

	static GCEventType GetStaticType() { return GCEventType::WindowResize; }
	GCEventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "WindowResizeEvent"; }

	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

private:
	unsigned int m_width, m_height;
};
#pragma endregion
