#include "pch.h"
#include "Bullet.h"

void GCScriptBullet::Start()
{
    m_destroyed = false;
}


void GCScriptBullet::CopyTo(GCComponent * pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptBullet * pNewComponent = static_cast<GCScriptBullet*>(pDestination);
    pNewComponent->m_attackSpeed = m_attackSpeed;
    pNewComponent->m_direction = m_direction;
    pNewComponent->m_pCamera = m_pCamera;
    pNewComponent->m_pOrigin = m_pOrigin;
}

void GCScriptBullet::Update()
{
        m_pGameObject->m_transform.Translate(m_direction);

        //Destroy Bullet when out of screen
        if (m_pGameObject->m_transform.m_position.x <= m_pCamera->GetPosition().x - m_pCamera->GetViewWidth()
            || m_pGameObject->m_transform.m_position.y <= m_pCamera->GetPosition().y - m_pCamera->GetViewHeight() / 2
            || m_pGameObject->m_transform.m_position.x >= m_pCamera->GetPosition().x + m_pCamera->GetViewWidth()
            || m_pGameObject->m_transform.m_position.y >= m_pCamera->GetPosition().y + m_pCamera->GetViewHeight() / 2)
        {
            m_pGameObject->Destroy();
        }
}

void GCScriptBullet::BulletShoot()
{
    m_attackSpeed = 0.1f;

    m_pGameObject->m_transform.SetPosition(m_pOrigin->m_transform.m_position);

    m_pCamera = GC::GetActiveScene()->GetMainCamera();

    m_direction = GCVEC3::Zero();
    m_direction.x = GCINPUTS::GetMousePos().x + rand() % 51;
    m_direction.y = GCINPUTS::GetMousePos().y + rand() % 51;

    m_direction -= m_pGameObject->m_transform.m_position * 100;
    m_direction.Normalize();
    m_direction *= m_attackSpeed;    
}

void GCScriptBullet::OnTriggerStay(GCCollider* pCollider)
{
    if (pCollider->GetGameObject()->HasTag("enemy") && m_destroyed == false)
    {
        pCollider->GetGameObject()->GetComponent<GCScriptEnemyBehaviour>()->Die();
        m_pGameObject->Destroy();
        m_destroyed = true;
    }
}
