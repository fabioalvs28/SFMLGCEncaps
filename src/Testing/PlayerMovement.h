#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START( PlayerMovement )

private:
    void Start() override;
    void Update() override;
    
    float m_speed;

CREATE_SCRIPT_END