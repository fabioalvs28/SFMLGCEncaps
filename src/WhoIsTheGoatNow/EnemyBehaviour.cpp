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
    //Spawn();
}

void GCScriptEnemyBehaviour::SetTarget(GCGameObject* pTarget)
{
    m_pTarget = pTarget;
}

void GCScriptEnemyBehaviour::FixedUpdate()
{
    GCVEC3 direction = m_pTarget->m_transform.m_position;
    direction -= m_pGameObject->m_transform.m_position;
    direction.Normalize();
    direction *= m_speed;

    m_pGameObject->m_transform.Translate(direction);
    std::cout << m_pGameObject->m_transform.m_position.x << m_pGameObject->m_transform.m_position.y << std::endl;
}

void GCScriptEnemyBehaviour::Spawn()
{
    float x, y;
    
    //Generate x and y outside of screen
    if (rand() % 2 == 0) {
        x = -10.0f - static_cast<float>(rand()) / RAND_MAX * (Window::GetApp()->GetClientWidth() / 2.0f - 10.0f);
    }
    else {
        x = 10.0f + static_cast<float>(rand()) / RAND_MAX * (Window::GetApp()->GetClientWidth() / 2.0f - 10.0f);
    }

    if (rand() % 2 == 0) {
        y = -10.0f - static_cast<float>(rand()) / RAND_MAX * (Window::GetApp()->GetClientHeight() / 2.0f - 10.0f);
    }
    else {
        y = 10.0f + static_cast<float>(rand()) / RAND_MAX * (Window::GetApp()->GetClientHeight() / 2.0f - 10.0f);
    }

    //Apply to transform
    m_pGameObject->m_transform.m_position.x = x;
    m_pGameObject->m_transform.m_position.y = y;

}

void GCScriptEnemyBehaviour::Die()
{
    Destroy();
}