#pragma once
#include <string>
#include<functional>

//macro to bind the event function
//Reminders: if use in a class, use GC_BIND_EVENT_FN(ClassName::FunctionName)
#define GC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

enum class GCEventType
{
	WindowClose, WindowResize,
	KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScrolled,
    Count //Keep this at the end
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

	bool isHandled = false;
};

class GCEventDispatcher
{
	template<typename T>
	using GCEventFn = std::function<bool(T&)>;
public:
	GCEventDispatcher(GCEvent& gcevent) : m_gcEvent(gcevent) {};

	template<typename Type>
	bool Dispatch(GCEventFn<Type> func)
	{
		if (m_gcEvent.GetEventType() == Type::GetStaticType())
		{
			m_gcEvent.isHandled = func(*(Type*)(&m_gcEvent));
			return true;
		}
		return false;
	}

private:
	GCEvent& m_gcEvent;
};

#pragma region MouseEvent
class GCMouseButtonPressed : public GCEvent
{
public:
	GCMouseButtonPressed(float x, float y, GCMouseButton mouseButton)
		: m_x(x), m_y(y), m_mouseButton(mouseButton) {}

	static GCEventType GetStaticType() { return GCEventType::MouseButtonPressed; }
	GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "MouseButtonPressed"; }

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }
	GCMouseButton GetMouseButton() const { return m_mouseButton; }

private:
	float m_x, m_y; //Maybe later to use the custom vector2
	GCMouseButton m_mouseButton;
};

class GCMouseMoveEvent : public GCEvent
{
public:
    GCMouseMoveEvent(float x, float y)
        : m_x(x), m_y(y)
    {
    }

    static GCEventType GetStaticType() { return GCEventType::MouseMove; }
    GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "MouseMove"; }

	float GetX() const { return m_x; }
	float GetY() const { return m_y; }

private:
    float m_x, m_y;

};

#pragma endregion

///
///
/// Events Related to Window
/// 
///
#pragma region WindowEvents
class GCWindowCloseEvent : public GCEvent {
public:
	GCWindowCloseEvent() {}

    static GCEventType GetStaticType() { return GCEventType::WindowClose; }
	GCEventType GetEventType() const override { return GetStaticType(); }
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

class GCKeyEvent : public GCEvent
{
public:
	GCKeyEvent(int id) : keyID(id) {}

protected:
	int keyID;
};

class GCKeyPressedEvent : public GCKeyEvent
{
public:
    GCKeyPressedEvent(int id) : GCKeyEvent(id) {};
	
    static GCEventType GetStaticType() { return GCEventType::KeyPressed; }
    GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "KeyPressedEvent"; }

    int GetKeyID() const { return keyID; }
};

class GCKeyReleased : public GCKeyEvent
{
public:
    GCKeyReleased(int id) : GCKeyEvent(id) {};

    static GCEventType GetStaticType() { return GCEventType::KeyReleased; }
    GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "KeyReleased"; }

    int GetKeyID() const { return keyID; }
};
