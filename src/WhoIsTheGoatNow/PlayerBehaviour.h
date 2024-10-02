#pragma once
#include "pch.h"

enum WeaponSelect
{
	Sniper ,
	Shotgun ,
	MachineGun
};

CREATE_SCRIPT_START(PlayerBehaviour)
public:
	void Start() override;
	void FixedUpdate() override;

	void SetWeapon( int weapon );
private:
	int m_ammunitions;
	float m_velocity;

	GCAnimator* m_pAnimator;
	InputSystem* m_pInputSystem;
	GCGameObject* m_pBulletTemplate;

CREATE_SCRIPT_END