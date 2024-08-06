#pragma once

#include <Windows.h>
#include "framework.h"

#include "GC.h"

#include "GameManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectTransform.h"
#include "Components.h"
#include "InputManager.h"
#include "Timer.h"
#include "Event.h"
#include "EventManager.h"

namespace GCINPUT
{

////////////////////////////////////////////////////////////
/// @brief Bind a function based on key ID and key state.
/// 
/// @tparam Func 
/// 
/// @param keyId 
/// @param state 
/// @param func 
////////////////////////////////////////////////////////////
template<typename Func>
void BindKey( KEYBOARD::Keys keyId, GCKeyboardInputManager::KeyboardState state, Func&& func )
{ GC::GetActiveInputSystem()->m_pKeyboard->BindAction( keyId, state, func ); }

//////////////////////////////////////////////////////////////
/// @brief Unbind a function based on key ID and key state.
/// 
/// @tparam Func 
/// 
/// @param keyId 
/// @param state 
/// @param func 
//////////////////////////////////////////////////////////////
template<typename Func>
void UnbindKey( KEYBOARD::Keys keyId, GCKeyboardInputManager::KeyboardState state, Func&& func )
{ GC::GetActiveInputSystem()->m_pKeyboard->UnbindAction( keyId, state, func ); }
};

namespace EVENT
{
    int CreateEventHandler( EventHandler* pHandler );
    void CallEventHandler( int handlerID );
    void PushEvent( GCEvent* pEvent );

    template<typename Event>
    void Subscribe( GCEventType eventType, void (*function)(Event&) )
    { GC::GetActiveEventManager()->Subscribe( eventType, function ); }
};

