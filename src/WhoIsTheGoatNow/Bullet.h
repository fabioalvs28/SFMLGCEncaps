#pragma once
#include "pch.h"

CREATE_SCRIPT_START(Bullet)
public:
	void Start() override;
	void FixedUpdate() override;
	void BulletShoot();
	void SetOrigin(GCGameObject* pOrigin) { m_pOrigin = pOrigin; };
	void OnTriggerStay(GCCollider* pCollider) override;
	
private:
	float m_attackSpeed;
	bool m_destroyed;
	GCVEC3 m_direction;
	GCCamera* m_pCamera;
	GCGameObject* m_pOrigin;

CREATE_SCRIPT_END