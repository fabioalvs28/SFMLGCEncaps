#pragma once

#include "GC.h"

#include "GameManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameObjectTransform.h"
#include "Components.h"
#include "InputManager.h"
#include "Timer.h"



template<typename Func>
void BINDKEY(GCKeyboardInputManager::KeyboardID keyId, GCKeyboardInputManager::KeyboardState state, Func&& func)
{
    GC::GetActiveInputSystem()->m_pKeyboard->BindAction(keyId, state, func);
}
