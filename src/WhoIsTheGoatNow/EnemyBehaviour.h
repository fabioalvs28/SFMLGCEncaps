#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemyBehaviour)
public:
	void Start() override;
	void FixedUpdate() override;

	void Die();

	void SetTarget(GCGameObject* pTarget);
	void Spawn();
private:
	int m_hp;
	float m_speed;
	GCGameObject* m_pTarget;
	
CREATE_SCRIPT_END