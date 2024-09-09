#include "pch.h"
#include "EnemySpawner.h"

void GCScriptEnemySpawner::CopyTo(GCComponent * pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptEnemySpawner * pNewComponent = static_cast<GCScriptEnemySpawner*>(pDestination);
}

void GCScriptEnemySpawner::Start()
{
	m_counter = 0.0f;
}

void GCScriptEnemySpawner::FixedUpdate()
{
	SpawnEnemies(GC::GetActiveTimer()->FixedDeltaTime());
}

void GCScriptEnemySpawner::SpawnEnemies(float deltaTime)
{
	m_counter += deltaTime;
	if (m_counter > 10.0f)
	{
		m_counter = 0.0f;
		GCGameObject* newEnemy = m_pEnemyTemplate->Duplicate();
		newEnemy->Activate();

		newEnemy->GetComponent<GCScriptEnemyBehaviour>()->SetTarget(
						m_pEnemyTemplate->GetComponent<GCScriptEnemyBehaviour>()->GetTarget());
	}
}

