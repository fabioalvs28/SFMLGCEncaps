#include "pch.h"
#include "Bullet.h"
#include "Weapon.h"
# define M_PIl          3.141592653589793238462643383279502884L 


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
    pNewComponent->m_pPlayer = m_pPlayer;
}

void GCScriptBullet::FixedUpdate()
{

    m_pGameObject->m_transform.Translate(m_direction);
    //m_pGameObject->m_transform.RotateRoll(0.05f);

    //Destroy Bullet when out of screen
    if (m_pGameObject->m_transform.m_position.x <= m_pCamera->GetPosition().x - m_pCamera->GetViewWidth() / 2
        || m_pGameObject->m_transform.m_position.y <= m_pCamera->GetPosition().y - m_pCamera->GetViewHeight() / 2
        || m_pGameObject->m_transform.m_position.x >= m_pCamera->GetPosition().x + m_pCamera->GetViewWidth() / 2
        || m_pGameObject->m_transform.m_position.y >= m_pCamera->GetPosition().y + m_pCamera->GetViewHeight() / 2)
    {
        m_pGameObject->Destroy();
    }
}

void GCScriptBullet::Shoot()
{
    GCVEC3 position = m_pGameObject->m_transform.m_position;

    float rotationSpeed = 0.019f;

    float deltaX = GCINPUTS::GetMousePos().x - position.x;
    float deltaY = GCINPUTS::GetMousePos().y - position.y;

    float angle = atan2f( deltaX , deltaY );

    float angleDegrees = angle * ( 180.0f / M_PIl ) - 90;
    m_attackSpeed = 0.05f;

    m_pGameObject->GetComponent<GCSpriteRenderer>()->SetOrigin( Right );
    m_pGameObject->m_transform.SetPosition(m_pOrigin->m_transform.m_position); 
    m_pGameObject->m_transform.SetRotation(0.0f,0.0f,angleDegrees * rotationSpeed);
    m_pCamera = GC::GetActiveScene()->GetMainCamera();

    m_direction = GCVEC3::Zero();
    m_direction.x = GCINPUTS::GetMousePos().x;
    m_direction.y = GCINPUTS::GetMousePos().y;

    m_direction -= m_pGameObject->m_transform.m_position;
    m_direction.Normalize();
    m_direction *= m_attackSpeed;    
    m_pGameObject->m_transform.Translate( m_direction * 20.0f); 
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
