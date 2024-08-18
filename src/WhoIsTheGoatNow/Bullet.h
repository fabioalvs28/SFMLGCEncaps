#pragma once
#include "pch.h"

CREATE_SCRIPT_START(Bullet)
public:
	void Shoot();

private:
	float m_attackSpeed;
CREATE_SCRIPT_END