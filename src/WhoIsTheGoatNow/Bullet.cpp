#include "pch.h"
#include "Bullet.h"

void GCScriptBullet::CopyTo(GCComponent * pDestination)
{
    GCComponent::CopyTo(pDestination);
    GCScriptBullet * pNewComponent = static_cast<GCScriptBullet*>(pDestination);
}

void GCScriptBullet::Shoot()
{
    GCVEC3 direction = GCVEC3::Zero();
    direction.x = GCINPUTS::GetMousePos().x;
    direction.y = GCINPUTS::GetMousePos().y;

    direction -= m_pGameObject->m_transform.m_position;
    direction.Normalize();
    direction *= m_attackSpeed;

    m_pGameObject->m_transform.Translate(direction);

    GCBoxCollider* pCollider = m_pGameObject->GetComponent<GCBoxCollider>();
    
    
    //#TODO Destroy Bullet when out of screen
}
