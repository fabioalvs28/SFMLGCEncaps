#include "pch.h"
#include "Weapon.h"

# define M_PIl          3.141592653589793238462643383279502884L 

void GCScriptWeapon::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptWeapon* pNewComponent = static_cast<GCScriptWeapon*>(pDestination);
	pNewComponent->m_direction = m_direction;
	pNewComponent->m_distanceToPlayer = m_distanceToPlayer;
}

void GCScriptWeapon::Start()
{
	m_distanceToPlayer = 1.0f;
	m_isFlipped = false;

}

void GCScriptWeapon::Update()
{
	
	GCVEC3* position = &m_pGameObject->GetParent()->m_transform.m_position;
	GCMATRIX* currentRotationMatrix = &m_pGameObject->GetParent()->m_transform.GetWorldRotationMatrix();

	float rotationSpeed = 0.019f;

	float deltaX = GCINPUTS::GetMousePos().x - position->x;
	float deltaY = GCINPUTS::GetMousePos().y - position->y;

	float angle = atan2f(deltaX, deltaY);

	float angleDegrees = angle * (180.0f / M_PIl) - 90;

	m_direction = GCVEC3::Zero();
	m_direction.x = GCINPUTS::GetMousePos().x;
	m_direction.y = GCINPUTS::GetMousePos().y;

	m_direction -= m_pGameObject->GetParent()->m_transform.m_position;
	m_direction.Normalize();
	m_direction *= m_distanceToPlayer;
	if ( GCINPUTS::GetMousePos().x < 0 && m_isFlipped == false)
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = true;
	}
	else if (GCINPUTS::GetMousePos().x > 0 && m_isFlipped)
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = false;
	}


    m_pGameObject->m_transform.SetPosition(m_direction);
	m_pGameObject->m_transform.SetRotation(0.0f,0.0f, angleDegrees * rotationSpeed);
}