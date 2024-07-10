#pragma once

#include "Components.h"

class GCGameObject;

class GCPhysicManager
{
friend class GCGameObject;

private:
	/*
	 * Components that need to be fixed updated
	 */
	GCList<Component*> m_components;

	/*
	 * Colliders that need to be checked for collision
	 */
	GCList<Collider*> m_colliders;
public:
	GCPhysicManager();
	~GCPhysicManager();

	void RegisterComponent(Component* pComponent);

	void RegisterCollider(Collider* pCollider);

	void Update();

private:
	bool CheckCollision(Collider& collider1, Collider& collider2);
};

namespace GCPhysic
{
	bool CheckBox2DvsBox2D(Collider& collider1, Collider& collider2);
	bool CheckBox2DvsCircle(Collider& collider1, Collider& collider2);
	bool CheckCirclevsCircle(Collider& collider1, Collider& collider2);
}