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
	void Shoot();
	void SetBulletTemplate( GCGameObject* bulletTemplate ) { m_pBulletTemplate = bulletTemplate; }

private:
	GCVEC3 m_direction;
	float m_distanceToPlayer;
	float m_interpolatedRotation;
	bool m_isFlipped;
	GCVEC3 m_centralizer;
	InputSystem* m_pInputSystem;
	GCGameObject* m_pBulletTemplate;

protected:
	int m_penetration;
	float m_range;
	float m_speed;

	float m_shootCooldown;

CREATE_SCRIPT_END

CREATE_SCRIPT_INHERIT_START(Sniper, Weapon)
public:
	void Start() override;


CREATE_SCRIPT_END