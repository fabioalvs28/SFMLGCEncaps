#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(Movable)

public:
    GCKeyboardInputManager* m_pKeyboard;
    GCMouseInputManager* m_pMouse;

private:
    void Start() override {};
    void Update() override;

CREATE_SCRIPT_END