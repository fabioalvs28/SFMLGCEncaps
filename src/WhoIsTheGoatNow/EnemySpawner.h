#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemySpawner)
public:
	void Start() override;
	void FixedUpdate() override;

	void SetTemplate(GCGameObject* pEnemy) { m_pEnemyTemplate = pEnemy; };
	void SpawnEnemies(float deltaTime);
	
private:
	GCGameObject* m_pEnemyTemplate;
	std::vector<GCGameObject*> m_pEnemies;
	float m_counter;

CREATE_SCRIPT_END