#pragma once

CREATE_SCRIPT_INHERIT_START(DumbGoat, EnemyBehaviour)

void Update() override;

protected:
	GCAnimator* m_pAnimator;
	bool m_isDead;

CREATE_SCRIPT_END