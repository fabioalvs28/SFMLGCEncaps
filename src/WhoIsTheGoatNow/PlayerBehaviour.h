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
	void Update() override;

	void SetWeapon( int weapon );
	void SetPauseScene(GCScene* pScene) { m_pPauseScene = pScene; }

private:
	int m_ammunitions;
	float m_velocity;

	GCScene* m_pPauseScene;
	GCAnimator* m_pAnimator;
	InputSystem* m_pInputSystem;
	GCGameObject* m_pBulletTemplate;

CREATE_SCRIPT_END