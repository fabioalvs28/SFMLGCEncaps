#pragma once
#include "pch.h"

// TODO Turn every methods into private methods

class GCPhysicManager
{
friend class GCGameObject;

public:
	GCPhysicManager();
	~GCPhysicManager();
	void RegisterComponent( GCComponent* pComponent );
	void RegisterCollider( GCCollider* pCollider );
	
	void Update();

private:
	bool CheckCollision( GCCollider* pFirst, GCCollider* pSecond );

private:
	GCList<GCComponent*> m_componentsList; // A list of pointers to the Components that need to be fixed updated
	GCList<GCCollider*> m_collidersList; // A list of pointers to the Colliders that need to be checked for collisions

};

namespace GCPhysic
{
	bool CheckBox2DvsBox2D( GCBoxCollider* pFirst, GCBoxCollider* pSecond );
	bool CheckBox2DvsCircle( GCBoxCollider* pBox, GCCircleCollider* pCircle );
	bool CheckCirclevsCircle( GCCircleCollider* pFirst, GCCircleCollider* pSecond );
}