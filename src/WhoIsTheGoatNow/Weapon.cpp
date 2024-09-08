#include "pch.h"
#include "Weapon.h"

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
}

void GCScriptWeapon::Update()
{
	
	GCVEC3* position = &m_pGameObject->GetParent()->m_transform.m_position;
	GCMATRIX* currentRotationMatrix = &m_pGameObject->GetParent()->m_transform.GetWorldRotationMatrix();

	float rotationSpeed = 0.019f;

	float deltaX = GCINPUTS::GetMousePos().x - position->x;
	float deltaY = GCINPUTS::GetMousePos().y - position->y;

	float angle = atan2(deltaX, deltaY);

	float angleDegrees = angle * (180.0f / 3.141592653589793238463);

	//float currentRotation = atan2(currentRotationMatrix->_11, currentRotationMatrix->_21) * (180.0f / 3.141592653589793238463);

	//float angleInterpolated = currentRotation + (angleDegrees - currentRotation) * rotationSpeed;

	m_direction = GCVEC3::Zero();
	m_direction.x = GCINPUTS::GetMousePos().x;
	m_direction.y = GCINPUTS::GetMousePos().y;

	m_direction -= m_pGameObject->GetParent()->m_transform.m_position;
	m_direction.Normalize();
	m_direction *= m_distanceToPlayer;

	m_pGameObject->m_transform.SetPosition(m_direction);
	//m_pGameObject->m_transform.SetRotation(0.0f,0.0f, angleDegrees * rotationSpeed - 90);
	m_pGameObject->m_transform.Rotate(0.0f,0.0f, 0.01f);
}