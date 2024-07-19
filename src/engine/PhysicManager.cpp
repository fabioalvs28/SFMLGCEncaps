#include "pch.h"
#include "PhysicManager.h"

#include <algorithm>

#include "Log.h"
#include "GameObject.h"

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
			if (!CheckCollision(*pColliderNode->GetData(), *pNextColliderNode->GetData()))
				continue;

			// Resolve collision
			LogEngineDebug("Collision detected");
		}
	}
}

bool GCPhysicManager::CheckCollision(Collider& collider1, Collider& collider2)
{
	switch (collider1.GetID())
	{
	case 2:
		if (collider2.GetID() == 2)
			return GCPhysic::CheckBox2DvsBox2D(collider1, collider2);
		else if (collider2.GetID() == 3)
			return GCPhysic::CheckBox2DvsCircle(collider1, collider2);
		break;

	case 3:
		if (collider2.GetID() == 2)
			return GCPhysic::CheckBox2DvsCircle(collider2, collider1);
		else if (collider2.GetID() == 3)
			return GCPhysic::CheckCirclevsCircle(collider1, collider2);
		break;
	default:
		LogEngineError("Invalid collider");
		break;
	}

	return false;
}

namespace GCPhysic
{
	bool CheckBox2DvsBox2D(Collider& collider1, Collider& collider2)
	{
		GCTransform& transform1 = collider1.GetGameObject()->m_transform;
		GCTransform& transform2 = collider2.GetGameObject()->m_transform;

		GCVEC3 position1 = transform1.m_position;
		GCVEC3 position2 = transform2.m_position;

		GCVEC2 size1 = static_cast<BoxCollider&>(collider1).GetSize();
		GCVEC2 size2 = static_cast<BoxCollider&>(collider2).GetSize();

		bool collisionX = position1.x + size1.x >= position2.x && position2.x + size2.x >= position1.x;
		bool collisionY = position1.y + size1.y >= position2.y && position2.y + size2.y >= position1.y;

		return collisionX && collisionY;
	}

	bool CheckBox2DvsCircle(Collider& collider1, Collider& collider2)
	{
		GCTransform& transform1 = collider1.GetGameObject()->m_transform;
		GCTransform& transform2 = collider2.GetGameObject()->m_transform;

		GCVEC3 position1 = transform1.m_position;
		GCVEC3 position2 = transform2.m_position;

		GCVEC2 size1 = static_cast<BoxCollider&>(collider1).GetSize();
		float radius = static_cast<CircleCollider&>(collider2).GetRadius();

		GCVEC2 center = { position2.x + radius, position2.y + radius };

		GCVEC2 aabbHalfExtents = { size1.x * 0.5f, size1.y * 0.5f };
		GCVEC2 aabbCenter = { position1.x + aabbHalfExtents.x, position1.y + aabbHalfExtents.y };

		GCVEC2 difference = center - aabbCenter;
		GCVEC2 clamped{ std::clamp(difference.x, -aabbHalfExtents.x, aabbHalfExtents.x), std::clamp(difference.y, -aabbHalfExtents.y, aabbHalfExtents.y) };

		GCVEC2 closest = aabbCenter + clamped;
		difference = closest - center;

		return difference.GetNormSquared() < radius * radius;
	}

	bool CheckCirclevsCircle(Collider& collider1, Collider& collider2)
	{
		GCTransform& transform1 = collider1.GetGameObject()->m_transform;
		GCTransform& transform2 = collider2.GetGameObject()->m_transform;

		GCVEC3 position1 = transform1.m_position;
		GCVEC3 position2 = transform2.m_position;

		float radius1 = static_cast<CircleCollider&>(collider1).GetRadius();
		float radius2 = static_cast<CircleCollider&>(collider2).GetRadius();

		GCVEC2 center1 = { position1.x + radius1, position1.y + radius1 };
		GCVEC2 center2 = { position2.x + radius2, position2.y + radius2 };

		GCVEC2 difference = center2 - center1;

		return difference.GetNormSquared() < (radius1 + radius2) * (radius1 + radius2);
	}
}