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
	int i = 0;
	m_counter += deltaTime;
	if (m_counter > 100.0f)
	{
		m_counter = 0.0f;
		m_pEnemies.push_back(GC::GetActiveScene()->CreateGameObject());
		m_pEnemies[i] = m_pEnemyTemplate->Duplicate();
		//m_pEnemies[i]->m_transform.SetPosition(GCVEC3::Zero());
		m_pEnemies[i]->GetComponent<GCScriptEnemyBehaviour>()->SetTarget(
						m_pEnemyTemplate->GetComponent<GCScriptEnemyBehaviour>()->GetTarget());
		i++;
	}
}

