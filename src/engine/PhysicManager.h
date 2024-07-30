#pragma once
#include "pch.h"

// TODO Turn every methods into private methods

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
	bool CheckCollision( Collider* pFirst, Collider* pSecond );
};

namespace GCPhysic
{
	bool CheckBox2DvsBox2D( BoxCollider* pFirst, BoxCollider* pSecond );
	bool CheckBox2DvsCircle( BoxCollider* pBox, CircleCollider* pCircle );
	bool CheckCirclevsCircle( CircleCollider* pFirst, CircleCollider* pSecond );
}