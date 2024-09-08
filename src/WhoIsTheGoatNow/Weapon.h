#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(Weapon)

public:
	void Update() override;
	void Start() override;

private:
	GCVEC3 m_direction;
	float m_distanceToPlayer;
	float m_interpolatedRotation;
CREATE_SCRIPT_END