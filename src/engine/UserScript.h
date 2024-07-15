#pragma once
#include "Components.h"

CREATE_SCRIPT_START( Example )

public:
    void Say( const char* message );
    
    void Start() override;
    void Update() override;
    void FixedUpdate() override;
    void Destroy() override;
    
    void OnTriggerEnter( Collider* collider ) override;
    void OnTriggerStay( Collider* collider ) override;
    void OnTriggerExit( Collider* collider ) override;

CREATE_SCRIPT_END