#pragma once
#include "pch.h"

CREATE_SCRIPT_START(PlayerBehaviour)
public:
	void Start() override;
	void Update() override;

	void Shoot();

private:
	int m_ammunitions;
	InputSystem* m_pInputSystem;
	float m_velocity;

CREATE_SCRIPT_END