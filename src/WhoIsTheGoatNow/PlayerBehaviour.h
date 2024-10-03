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

	void GetWeapon();

	void SetDeathScene( GCScene* deathScene ) { m_pDeathScene = deathScene; };
	void SetUpgradeScene( GCScene* upgradeScene ) { m_pUpgradeScene = upgradeScene; };
	void SetCardTemplate( GCGameObject* pCard ) { m_pCardTemplate = pCard; }

	int GetHp() { return m_hp; }
	int GetLife() { return m_life; }
	int GetExp() { return m_exp; }
	int GetLevel() { return m_level; }
	int GetLevelAmount() { return m_levelAmount; }
	int GetWeaponIndex() { return m_weaponIndex; }

	void UpgradeVelocity( float value ) { m_velocity += value; }
	void UpgradeHP( int value ) { m_hp += value; }
	void UpgradeMaxHealth( int value ) { m_life += value; }

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

	GCScene* m_pPauseScene;
	GCAnimator* m_pAnimator;
	InputSystem* m_pInputSystem;
	GCGameObject* m_pCardTemplate;
	GCGameObject* m_pBulletTemplate;
	GCScene* m_pDeathScene;
	GCScene* m_pUpgradeScene;

CREATE_SCRIPT_END