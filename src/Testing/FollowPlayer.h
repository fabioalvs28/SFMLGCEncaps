#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START( FollowPlayer )

public:
    void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; }

private:
    void Update() override;

    GCGameObject* m_pPlayer;

CREATE_SCRIPT_END