#pragma once
#include "pch.h"

// macro to bind the event function
// Reminders: if use in a class, use GC_BIND_EVENT_FN(ClassName::FunctionName)
#define GC_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

enum class GCEventType
{
	WindowClose, WindowResize,
	KeyInput, KeyPressed, KeyReleased,
	MouseButtonPressed, MouseButtonReleased, MouseMove, MouseScrolled,
	ComponentAdded, ComponentRemove,
	Custom,
    Count // Keep this at the end
};

enum class GCMouseButton
{
	Left,
	Right,
	Middle
};

///////////////////////////////////////////////////////////////////
/// @brief Base event type class.
/// 
/// @note Derived class should have a static get method to work.
///////////////////////////////////////////////////////////////////
class GCEvent {
public:
	virtual GCEventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
	virtual ~GCEvent() = default;

	bool m_isHandled = false;
};

class GCEventDispatcher
{
	template<typename T>
	using GCEventFn = std::function<bool(T&)>;
public:
	GCEventDispatcher( GCEvent& gcevent ) : m_gcEvent( gcevent ) {};

	template<typename Type>
	bool Dispatch( GCEventFn<Type> func )
	{
		if ( m_gcEvent.GetEventType() == Type::GetStaticType() )
		{
			m_gcEvent.m_isHandled = func( *(Type*)( &m_gcEvent ) );
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
	GCMouseButtonPressed( float x, float y, GCMouseButton mouseButton ) : m_x( x ), m_y( y ), m_mouseButton( mouseButton ) {}

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
    GCMouseMoveEvent( float x, float y ) : m_x( x ), m_y( y ) {}

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

////////////////////////////////////////////////////////////////
/// @brief Event occurs when application's window is resized.
////////////////////////////////////////////////////////////////
class GCWindowResizeEvent : public GCEvent {
public:
	GCWindowResizeEvent( unsigned int width, unsigned int height ) : m_width( width ), m_height( height ) {}

	static GCEventType GetStaticType() { return GCEventType::WindowResize; }
	GCEventType GetEventType() const override { return GetStaticType(); }
	const char* GetName() const override { return "WindowResizeEvent"; }

	unsigned int GetWidth() const { return m_width; }
	unsigned int GetHeight() const { return m_height; }

private:
	unsigned int m_width;
	unsigned int m_height;
};
#pragma endregion

#pragma region KeyEvent

typedef unsigned char BYTE;

class GCKeyEvent : public GCEvent
{
public:
	GCKeyEvent( int id, BYTE state ) : m_keyID( id ), m_keyState( state ) {}

protected:
	int m_keyID;
	BYTE m_keyState;
};

class GCKeyPressedEvent : public GCKeyEvent
{
public:
    GCKeyPressedEvent( int id, BYTE state ) : GCKeyEvent( id, state ) {};
	
    static GCEventType GetStaticType() { return GCEventType::KeyPressed; }
    GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "KeyPressedEvent"; }

    int GetKeyID() const { return m_keyID; }
	BYTE GetKeyState() const { return m_keyState; }
};

class GCKeyReleasedEvent : public GCKeyEvent
{
public:

    GCKeyReleasedEvent( int id, BYTE state ) : GCKeyEvent( id, state ) {};

    static GCEventType GetStaticType() { return GCEventType::KeyReleased; }
    GCEventType GetEventType() const override { return GetStaticType(); }
    const char* GetName() const override { return "KeyReleased"; }

    int GetKeyID() const { return m_keyID; }
	BYTE GetKeyState() const { return m_keyState; }
};
#pragma endregion

class ComponentAddedEvent : public GCEvent
{
public:
	ComponentAddedEvent( GCComponent* pComponent ) : m_pComponent( pComponent ) {}
		
	static GCEventType GetStaticType() { return GCEventType::ComponentAdded; }
	GCEventType GetEventType() const override { return GetStaticType(); }

	GCComponent* GetComponent() const { return m_pComponent; }

private:
	GCComponent* m_pComponent;
};

template<typename... Args>
class CustomEvent : public GCEvent
{
public:
	CustomEvent( Args... args ) : m_data( std::make_tuple( args... ) ) { ++m_eventID; }

	static GCEventType GetStaticType() { return GCEventType::Custom; }
	GCEventType GetEventType() const override { return GetStaticType(); }

	template<std::size_t Index>
	auto GetData() const -> decltype( std::get<Index>( this->m_data ) )
	{
		return std::get<Index>( this->m_data );
	}

private:
	std::tuple<Args...> m_data;

protected:
	inline static const int m_ID = 0;
	int m_eventID;
};