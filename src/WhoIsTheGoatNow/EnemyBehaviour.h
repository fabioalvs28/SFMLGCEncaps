#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemyBehaviour)
public:
	void Start() override;
	void FixedUpdate() override;

	void Spawn();
	virtual void Die();

	void SetTarget(GCGameObject* pTarget) { m_pTarget = pTarget; };
	GCGameObject* GetTarget() { return m_pTarget; };

private:
	int m_hp;
	float m_speed;
	GCGameObject* m_pTarget;
	
CREATE_SCRIPT_END