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
    pNewComponent->m_startPosition = m_startPosition;
    pNewComponent->m_centralizer = m_centralizer;
}

void GCScriptBullet::FixedUpdate()
{
    //if (m_pPlayer == nullptr)
    //    return;

    m_pGameObject->m_transform.Translate(m_direction);
    float first = ( m_pGameObject->m_transform.m_position.x - m_pPlayer->m_transform.m_position.x + m_centralizer.x ) * ( m_pGameObject->m_transform.m_position.x - m_pPlayer->m_transform.m_position.x + m_centralizer.x );
    float second = ( m_pGameObject->m_transform.m_position.y - m_pPlayer->m_transform.m_position.y + m_centralizer.y ) * ( m_pGameObject->m_transform.m_position.y - m_pPlayer->m_transform.m_position.y + m_centralizer.y );
    //Destroy Bullet when out of screen
    if (std::pow(first + second, 0.5f) > m_range)
    {
        m_pGameObject->Destroy();
    }
}

void GCScriptBullet::Shoot(int bulletIndex)
{
    m_centralizer = m_pPlayer->m_transform.GetWorldScale() / 2;
    GCVEC3 m_startPosition = m_pPlayer->m_transform.m_position + m_centralizer;

    float rotationSpeed = 0.019f;

    float deltaX = GCINPUTS::GetMousePos().x + bulletIndex - m_startPosition.x;
    float deltaY = GCINPUTS::GetMousePos().y + bulletIndex - m_startPosition.y;

    float angle = atan2f( deltaX , deltaY );

    long double angleDegrees = angle * ( 180.0f / M_PIl ) - 90;

    m_pGameObject->GetComponent<GCSpriteRenderer>()->SetOrigin( Left );
    m_pGameObject->m_transform.SetPosition(m_pPlayer->m_transform.m_position + m_centralizer );
    m_pGameObject->m_transform.SetRotation(0.0f,0.0f,angleDegrees * rotationSpeed);
    m_pCamera = GC::GetActiveScene()->GetMainCamera();

    m_direction = GCVEC3::Zero();
    m_direction.x = GCINPUTS::GetMousePos().x + bulletIndex;
    m_direction.y = GCINPUTS::GetMousePos().y + bulletIndex;

    m_direction -= m_pPlayer->m_transform.m_position + m_centralizer;
    m_direction.Normalize();
    m_direction *= m_attackSpeed;    
    m_pGameObject->m_transform.Translate(m_direction);
}

void GCScriptBullet::OnTriggerStay(GCCollider* pCollider)
{
    if (pCollider->GetGameObject()->HasTag("enemy") && m_destroyed == false)
    {
        pCollider->GetGameObject()->GetComponent<GCScriptEnemyBehaviour>()->Die();
        if ( m_penetration <= 0 )
        {
            m_pGameObject->Destroy();
            m_destroyed = true;
        }
        else
        {
            m_penetration -= 1;
        }
    }
}
