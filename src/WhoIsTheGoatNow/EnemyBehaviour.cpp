#include "pch.h"
#include "EnemyBehaviour.h"

void GCScriptEnemyBehaviour::CopyTo(GCComponent * pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptEnemyBehaviour * pNewComponent = static_cast<GCScriptEnemyBehaviour*>(pDestination);
}

void GCScriptEnemyBehaviour::Start()
{
	m_hp = 1;
}

void GCScriptEnemyBehaviour::Update()
{
    GCVEC3 direction = m_pPlayer->m_transform.m_position;
    direction -= m_pGameObject->m_transform.m_position;
    direction.Normalize();
    direction *= m_speed;

    m_pGameObject->m_transform.Translate(direction);
}

void GCScriptEnemyBehaviour::Die()
{
    Destroy();
}