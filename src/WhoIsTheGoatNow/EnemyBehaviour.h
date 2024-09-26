#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemyBehaviour)
public:
    void Die();
    virtual void Start() override;
    void FixedUpdate() override;

    void Spawn();

public:
    void SetTarget(GCGameObject * pTarget) { m_pTarget = pTarget; };
    GCGameObject* GetTarget() { return m_pTarget; };

protected:
    int m_hp;
    float m_speed;
    GCVEC3 m_direction;
    GCGameObject* m_pTarget;

CREATE_SCRIPT_END