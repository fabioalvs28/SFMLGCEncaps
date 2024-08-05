#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(Example)

public:
    void ResetPosition();
    void PassTime();
    void ChangeTime();

private:
    void Start() override; // This method is called when the Script is created
    void Update() override; // This method is called every frame
    
    SpriteRenderer* m_pSpriteRenderer;
    bool m_night;
    unsigned int m_days;

CREATE_SCRIPT_END