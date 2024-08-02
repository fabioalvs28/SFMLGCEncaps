#pragma once

#include "GC.h"

#include "GameManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectTransform.h"
#include "Components.h"
#include "InputManager.h"
#include "Timer.h"

namespace GCInput
{
/// <summary>
/// Bind a function based on key ID and key state
/// </summary>
/// <typeparam name="Func"></typeparam>
/// <param name="keyId"></param>
/// <param name="state"></param>
/// <param name="func"></param>
template<typename Func>
void BindKey(KEYBOARD::Keys keyId, GCKeyboardInputManager::KeyboardState state, Func&& func)
{
    GC::GetActiveInputSystem()->m_pKeyboard->BindAction(keyId, state, func);
}

/// <summary>
/// Unbind a function based on key ID and key state
/// </summary>
/// <typeparam name="Func"></typeparam>
/// <param name="keyId"></param>
/// <param name="state"></param>
/// <param name="func"></param>
template<typename Func>
void UnbindKey(KEYBOARD::Keys keyId, GCKeyboardInputManager::KeyboardState state, Func&& func)
{
    GC::GetActiveInputSystem()->m_pKeyboard->UnbindAction(keyId, state, func);
}
};



