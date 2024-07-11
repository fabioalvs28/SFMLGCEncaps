#include "pch.h"
#include "Components.h"

#include "Log.h"

#include "GC.h"
#include "GameObject.h"


Component::Component( GCGameObject* pGameObject )
{
	m_flags = 0;
	m_active = true;
	m_pGameObject = pGameObject;
	
	m_pUpdateNode = nullptr;
	m_pPhysicsNode = nullptr;
	m_pRenderNode = nullptr;
	
	if ( IsFlagSet( UPDATE ) )
		;

	if ( IsFlagSet( FIXED_UPDATE ) )
		;

	if ( IsFlagSet( RENDER ) )
		;
}



#pragma region Collider
Collider::Collider( GCGameObject* pGameObject ) : Component( pGameObject )
{
	m_trigger = false;
	m_visible = false;
	GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);
}
#pragma endregion Collider

#pragma region RigidBody
RigidBody::RigidBody( GCGameObject* pGameObject ) : Component( pGameObject )
{
	m_velocity.SetZero();
}

void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody