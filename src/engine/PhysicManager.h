#pragma once

#include "Components.h"

class GCGameObject;

class GCPhysicManager
{
friend class GCGameObject;

private:
	GCVector<RigidBody*> m_rigidbodies;
	GCVector<Collider*> m_colliders;
public:
	GCPhysicManager();
	~GCPhysicManager();

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