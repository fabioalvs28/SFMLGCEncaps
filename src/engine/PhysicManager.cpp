#include "pch.h"


GCPhysicManager::GCPhysicManager() {}

GCPhysicManager::~GCPhysicManager() {}



void GCPhysicManager::RegisterComponent( GCComponent* pComponent )
{ pComponent->m_pPhysicsNode = m_componentsList.PushBack( pComponent ); }

void GCPhysicManager::RegisterCollider( GCCollider* pCollider )
{ pCollider->m_pColliderNode = m_collidersList.PushBack( pCollider ); }



void GCPhysicManager::Update()
{
	for ( GCListNode<GCComponent*>* pComponentNode = m_componentsList.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext() )
		pComponentNode->GetData()->FixedUpdate();

	for ( GCListNode<GCCollider*>* pColliderNode = m_collidersList.GetFirstNode(); pColliderNode != nullptr; pColliderNode = pColliderNode->GetNext() )
	{
		GCCollider* pCollider = pColliderNode->GetData();
		for ( GCListNode<GCCollider*>* pNextColliderNode = pColliderNode->GetNext(); pNextColliderNode != nullptr; pNextColliderNode = pNextColliderNode->GetNext() )
		{
			GCCollider* pNextCollider = pNextColliderNode->GetData();
			
			if ( CheckCollision( pCollider, pNextCollider ) == false )
				continue;
			
			LogEngineDebug( "Collision detected" );
			pCollider->m_pGameObject->OnTriggerStay( pNextCollider );
			pNextCollider->m_pGameObject->OnTriggerStay( pCollider );
		}
	}
}



bool GCPhysicManager::CheckCollision( GCCollider* pFirst, GCCollider* pSecond )
{
	if ( pFirst->GetID() == GCBoxCollider::GetIDStatic() )
		if ( pSecond->GetID() == GCBoxCollider::GetIDStatic() )
			return CheckCollision( static_cast<GCBoxCollider*>( pFirst ), static_cast<GCBoxCollider*>( pSecond ) );
	
	if ( pFirst->GetID() == GCCircleCollider::GetIDStatic() )
		if ( pSecond->GetID() == GCCircleCollider::GetIDStatic() )
			return CheckCollision( static_cast<GCCircleCollider*>( pFirst ), static_cast<GCCircleCollider*>( pSecond ) );
	
	return CheckCollision( static_cast<GCBoxCollider*>( pFirst ), static_cast<GCCircleCollider*>( pSecond ) );
}

bool GCPhysicManager::CheckCollision( GCBoxCollider* pFirst, GCBoxCollider* pSecond )
{
	GCTransform* pFirstTransform = &pFirst->GetGameObject()->m_transform;
	GCTransform* pSecondTransform = &pSecond->GetGameObject()->m_transform;

	GCVEC3 firstPosition = pFirstTransform->m_position;
	GCVEC3 secondPosition = pSecondTransform->m_position;
	
	GCVEC3 firstScale = pFirstTransform->m_scale;
	GCVEC3 secondScale = pSecondTransform->m_scale;

	bool collisionX = firstPosition.x + firstScale.x >= secondPosition.x && secondPosition.x + secondScale.x >= firstPosition.x;
	bool collisionY = firstPosition.y + firstScale.y >= secondPosition.y && secondPosition.y + secondScale.y >= firstPosition.y;

	return collisionX && collisionY;
}

bool GCPhysicManager::CheckCollision( GCBoxCollider* pBox, GCCircleCollider* pCircle )
{
	GCTransform* pBoxTransform = &pBox->GetGameObject()->m_transform;
	GCTransform* pCircleTransform = &pCircle->GetGameObject()->m_transform;

	GCVEC3 boxPosition = pBoxTransform->m_position;
	GCVEC3 circlePosition = pCircleTransform->m_position;
	
	GCVEC3 boxScale = pBoxTransform->m_scale;
	float circleRadius = pCircleTransform->m_scale.x * 0.5f; //! Radius is scaled with x and not y

	GCVEC2 center = { circlePosition.x + circleRadius, circlePosition.y + circleRadius };

	GCVEC2 aabbHalfExtents = { boxScale.x * 0.5f, boxScale.y * 0.5f };
	GCVEC2 aabbCenter = { boxPosition.x + aabbHalfExtents.x, boxPosition.y + aabbHalfExtents.y };

	GCVEC2 difference = center - aabbCenter;
	GCVEC2 clamped{ std::clamp( difference.x, -aabbHalfExtents.x, aabbHalfExtents.x ), std::clamp( difference.y, -aabbHalfExtents.y, aabbHalfExtents.y ) };

	GCVEC2 closest = aabbCenter + clamped;
	difference = closest - center;

	return difference.GetNormSquared() < circleRadius * circleRadius;
}

bool GCPhysicManager::CheckCollision( GCCircleCollider* pFirst, GCCircleCollider* pSecond )
{
	GCTransform* pFirstTransform = &pFirst->GetGameObject()->m_transform;
	GCTransform* pSecondTransform = &pSecond->GetGameObject()->m_transform;

	GCVEC3 firstPosition = pFirstTransform->m_position;
	GCVEC3 secondPosition = pSecondTransform->m_position;

	float firstRadius = pFirstTransform->m_scale.x * 0.5f;
	float secondRadius = pSecondTransform->m_scale.x * 0.5f;

	GCVEC2 firstCenter = { firstPosition.x + firstRadius, firstPosition.y + firstRadius };
	GCVEC2 secondCenter = { secondPosition.x + secondRadius, secondPosition.y + secondRadius };

	GCVEC2 difference = secondCenter - firstCenter;

	return difference.GetNormSquared() < ( firstRadius + secondRadius ) * ( firstRadius + secondRadius );
}