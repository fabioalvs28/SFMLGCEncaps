#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemyBehaviour)
public:
	void Start() override;
	void Update() override;

	void Die();

private:
	int m_hp;
	float m_speed;
	GCGameObject* m_pPlayer;
	
CREATE_SCRIPT_END