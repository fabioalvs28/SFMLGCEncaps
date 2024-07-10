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
	GCVector<Component*> m_components;

	/*
	 * TODO:
	 * Should be removed as we now have a list of colliders
	 */
	GCVector<RigidBody*> m_rigidbodies;

	/*
	 * Colliders that need to be checked for collision
	 */
	GCVector<Collider*> m_colliders;
public:
	GCPhysicManager();
	~GCPhysicManager();

	void RegisterComponent(Component* component);
	void UnregisterComponent(Component* component);

	void RegisterCollider(Collider* collider);
	void UnregisterCollider(Collider* collider);

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