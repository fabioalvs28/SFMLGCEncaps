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
	void GetWeapon();
	void SetDeathScene( GCScene* deathScene ) { m_pDeathScene = deathScene; };

	int GetHp() { return m_hp; }
	int GetLife() { return m_life; }
	int GetExp() { return m_exp; }
	int GetLevel() { return m_level; }

	void AddExp( int value ) { m_exp += value; }

	void LevelUp();
	void Hit(int damage);
	void Die();

private:
 	int m_hp;
	int m_life;
	int m_exp;
	int m_level;
	int m_levelAmount;
	int m_weaponIndex;
	float m_velocity;

	GCAnimator* m_pAnimator;
	InputSystem* m_pInputSystem;
	GCGameObject* m_pBulletTemplate;
	GCScene* m_pDeathScene;

CREATE_SCRIPT_END