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
    m_speed = 0.03f;
    m_spawning = false;
    m_destroyed = false;
    Spawn();
}

void GCScriptEnemyBehaviour::FixedUpdate()
{
    if (m_pTarget != NULL && m_spawning == false)
    {
        m_direction = m_pTarget->m_transform.m_position;
        m_direction -= m_pGameObject->m_transform.m_position;
        m_direction.Normalize();
        m_direction;

        m_pGameObject->m_transform.Translate(m_direction * m_speed);
    }
}

void GCScriptEnemyBehaviour::Spawn()
{
    float x, y;
    
    // Get the half dimensions of the screen
    float halfWidth = GC::GetActiveScene()->GetMainCamera()->GetViewWidth() / 2.0f;
    float halfHeight = GC::GetActiveScene()->GetMainCamera()->GetViewHeight() / 2.0f;

    // Randomly choose if x will be in the range [-15, -10] or [10, 15]
    if (rand() % 2 == 0) {
        // x is in the range [-15, -10]
        x = -15.0f + static_cast<float>(rand()) / RAND_MAX * 5.0f;
    }
    else {
        // x is in the range [10, 15]
        x = 10.0f + static_cast<float>(rand()) / RAND_MAX * 5.0f;
    }

    // Randomly choose if y will be in the range [-15, -10] or [10, 15]
    if (rand() % 2 == 0) {
        // y is in the range [-15, -10]
        y = -15.0f + static_cast<float>(rand()) / RAND_MAX * 5.0f;
    }
    else {
        // y is in the range [10, 15]
        y = 15.0f + static_cast<float>(rand()) / RAND_MAX * 5.0f;
    }

    //Apply to transform
    m_pGameObject->m_transform.Translate(GCVEC3(x, y, 0));

}

void GCScriptEnemyBehaviour::Die()
{
    m_pGameObject->Destroy();
    m_destroyed = true;
}

void GCScriptEnemyBehaviour::OnTriggerStay(GCCollider* pCollider)
{
    if (pCollider->GetGameObject()->HasTag("bullet") && m_destroyed == false)
        Die();
}