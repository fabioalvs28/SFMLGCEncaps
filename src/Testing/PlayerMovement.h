#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START( PlayerMovement )

private:
    void Start() override;
    void Update() override;
    
    float m_speed;
    GCSprite* m_pSprite1;
    GCSprite* m_pSprite2;

CREATE_SCRIPT_END