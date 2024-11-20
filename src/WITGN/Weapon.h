#pragma once
#include "GCEngine.h"

enum OriginType
{
	BottomLeft ,
	BottomRight ,
	TopLeft ,
	TopRight ,
	Center ,
	Left ,
	Right ,
	Top ,
	Bottom
};

CREATE_SCRIPT_START(Weapon)

public:
	void Update() override;
	void Start() override;
	virtual void Shoot();

	void SetBulletTemplate( GCGameObject* bulletTemplate ) { m_pBulletTemplate = bulletTemplate; }
	void SetBulletSprite( GCSprite* bulletSprite ) { m_pBulletSprite = bulletSprite; };

	void UpgradePenetration( int value ) { m_penetration += value; };
	void UpgradeRange( float value ) { m_range += value; };
	void UpgradeAttackSpeed( float value ) { m_attackSpeed += value; };
	void UpgradeShootCooldown( float value ) { m_shootCooldown += value; };

private:
	float m_distanceToPlayer;
	float m_interpolatedRotation;

protected:
	GCGameObject* m_pBulletTemplate;

	float m_currentDeltaTime;
	bool m_isFlipped;
	GCVEC3 m_centralizer;
	InputSystem* m_pInputSystem;

	int m_penetration;
	float m_range;
	float m_attackSpeed;

	float m_shootCooldown;

	GCSprite* m_pBulletSprite;

CREATE_SCRIPT_END

CREATE_SCRIPT_INHERIT_START(Sniper, Weapon)
public:
	void Start() override;

CREATE_SCRIPT_END

CREATE_SCRIPT_INHERIT_START(MachineGun, Weapon)
public:
	void Start() override;
	void Update() override;

CREATE_SCRIPT_END

CREATE_SCRIPT_INHERIT_START(Shotgun, Weapon)
public:
	void Start() override;
	void Shoot() override;

private:
	int m_bulletAmount;

CREATE_SCRIPT_END