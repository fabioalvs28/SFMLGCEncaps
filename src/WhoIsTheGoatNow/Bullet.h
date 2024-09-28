#pragma once
#include "pch.h"

CREATE_SCRIPT_START(Bullet)
public:
	void Start() override;
	void FixedUpdate() override;
	void Shoot();
	void OnTriggerStay(GCCollider* pCollider) override;
	
	void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; }
	void SetOrigin( GCGameObject* pOrigin ) { m_pOrigin = pOrigin; }

	void SetAttackSpeed( float attackSpeed ) { m_attackSpeed = attackSpeed; }
	void SetRange( float range ) { m_range = range; }
	void SetPenetration( int penetration ) { m_penetration = penetration; }

private:
	float m_attackSpeed;
	float m_range;
	int m_penetration;

	bool m_destroyed;
	bool m_isFlipped;
	float m_startRotationZ;

	GCVEC3 m_startPosition;
	GCVEC3 m_direction;
	GCCamera* m_pCamera;
	GCGameObject* m_pPlayer;
	GCGameObject* m_pOrigin;

CREATE_SCRIPT_END