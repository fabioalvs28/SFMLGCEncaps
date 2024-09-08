#pragma once
#include "pch.h"

CREATE_SCRIPT_START(PlayerBehaviour)
public:
	void Start() override;
	void FixedUpdate() override;
	void Update() override;

	void PlayerShoot();
	void SetBulletTemplate(GCGameObject* pBullet) { m_pBulletTemplate = pBullet; };

private:
	int m_ammunitions;
	float m_velocity;

	InputSystem* m_pInputSystem;
	GCGameObject* m_pBulletTemplate;

CREATE_SCRIPT_END