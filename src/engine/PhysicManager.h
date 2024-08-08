#pragma once
#include "pch.h"

class GCPhysicManager
{
friend class GCComponent;
friend class GCCollider;
friend class GCGameObject;
friend class GCGameManager;

private:
	GCPhysicManager();
	~GCPhysicManager();
	
	void RegisterComponent( GCComponent* pComponent );
	void RegisterCollider( GCCollider* pCollider );
	
	void Update();
	
	bool CheckCollision( GCCollider* pFirst, GCCollider* pSecond );
	bool CheckCollision( GCBoxCollider* pFirst, GCBoxCollider* pSecond );
	bool CheckCollision( GCBoxCollider* pBox, GCCircleCollider* pCircle );
	bool CheckCollision( GCCircleCollider* pFirst, GCCircleCollider* pSecond );

private:
	GCList<GCComponent*> m_componentsList; // A list of pointers to the Components that need to be fixed updated
	GCList<GCCollider*> m_collidersList; // A list of pointers to the Colliders that need to be checked for collisions

};