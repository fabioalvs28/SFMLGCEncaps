#include "pch.h"


GCPhysicManager::GCPhysicManager()
{
}

GCPhysicManager::~GCPhysicManager()
{
}

void GCPhysicManager::RegisterComponent(Component* component)
{
	m_components.PushBack(component);
}

void GCPhysicManager::RegisterCollider(Collider* collider)
{
	m_colliders.PushBack(collider);
}

void GCPhysicManager::Update()
{
	for (GCListNode<Component*>* pComponentNode = m_components.GetFirstNode(); pComponentNode != nullptr; pComponentNode = pComponentNode->GetNext())
		pComponentNode->GetData()->FixedUpdate();

	for (GCListNode<Collider*>* pColliderNode = m_colliders.GetFirstNode(); pColliderNode != nullptr; pColliderNode = pColliderNode->GetNext())
	{
		for (GCListNode<Collider*>* pNextColliderNode = pColliderNode->GetNext(); pNextColliderNode != nullptr; pNextColliderNode = pNextColliderNode->GetNext())
		{
			if ( CheckCollision(pColliderNode->GetData(), pNextColliderNode->GetData()) == false )
				continue;

			// Resolve collision
			LogEngineDebug("Collision detected");
		}
	}
}

bool GCPhysicManager::CheckCollision( Collider* pFirst, Collider* pSecond )
{
	if ( pFirst->GetID() == BoxCollider::GetIDStatic() )
		if ( pSecond->GetID() == BoxCollider::GetIDStatic() )
			return GCPhysic::CheckBox2DvsBox2D( static_cast<BoxCollider*>(pFirst), static_cast<BoxCollider*>(pSecond) );
	
	if ( pFirst->GetID() == CircleCollider::GetIDStatic() )
		if ( pSecond->GetID() == CircleCollider::GetIDStatic() )
			return GCPhysic::CheckCirclevsCircle( static_cast<CircleCollider*>(pFirst), static_cast<CircleCollider*>(pSecond) );
	
	return GCPhysic::CheckBox2DvsCircle( static_cast<BoxCollider*>(pFirst), static_cast<CircleCollider*>(pSecond) );
}

namespace GCPhysic
{
	bool CheckBox2DvsBox2D( BoxCollider* pFirst, BoxCollider* pSecond )
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

	bool CheckBox2DvsCircle( BoxCollider* pBox, CircleCollider* pCircle )
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
		GCVEC2 clamped{ std::clamp(difference.x, -aabbHalfExtents.x, aabbHalfExtents.x), std::clamp(difference.y, -aabbHalfExtents.y, aabbHalfExtents.y) };

		GCVEC2 closest = aabbCenter + clamped;
		difference = closest - center;

		return difference.GetNormSquared() < circleRadius * circleRadius;
	}

	bool CheckCirclevsCircle( CircleCollider* pFirst, CircleCollider* pSecond )
	{
		GCTransform* pFirstTransform = &pFirst->GetGameObject()->m_transform;
		GCTransform* pSecondTransform = &pSecond->GetGameObject()->m_transform;

		GCVEC3 firstPosition = pFirstTransform->m_position;
		GCVEC3 secondPosition = pSecondTransform->m_position;

		float firstRadius = pFirstTransform->m_scale.x * 0.5f;
		float secondRadius = pSecondTransform->m_scale.x * 0.5f;

		GCVEC2 center1 = { firstPosition.x + firstRadius, firstPosition.y + firstRadius };
		GCVEC2 center2 = { secondPosition.x + secondRadius, secondPosition.y + secondRadius };

		GCVEC2 difference = center2 - center1;

		return difference.GetNormSquared() < (firstRadius + secondRadius) * (firstRadius + secondRadius);
	}
}