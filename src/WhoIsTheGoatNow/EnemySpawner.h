#pragma once
#include "pch.h"

CREATE_SCRIPT_START(EnemySpawner)
public:
	void Start() override;
	void FixedUpdate() override;

	void SetTemplate(GCGameObject* pEnemy) { m_pEnemyTemplate = pEnemy; }
	void AddEnemyInList(int enemyID, GCGameObject* pEnemy) { m_pEnemies.insert(std::pair<int, GCGameObject*>(enemyID, pEnemy)); }
	void SpawnEnemies(float deltaTime);
	
private:
	GCGameObject* m_pEnemyTemplate;
	std::map<int,GCGameObject*> m_pEnemies;
	GCText* m_pText;
	float m_counter;
	int m_enemyCount;

CREATE_SCRIPT_END