#include "pch.h"
#include "Components.h"

#include "Log.h"
#include "GameObject.h"
#include "GC.h"



Component::Component()
{
	m_active = true;
	m_pGameObject = nullptr;
	
	m_pUpdateNode = nullptr;
	m_pPhysicsNode = nullptr;
	m_pRenderNode = nullptr;
}



void Component::RegisterToManagers()
{
	if ( IsFlagSet( UPDATE ) )
		;
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		;
	
	if ( IsFlagSet( RENDER ) )
		;
}



void Component::UnregisterFromManagers()
{
	if ( IsFlagSet( UPDATE ) )
		;
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		;
	
	if ( IsFlagSet( RENDER ) )
		;
}



#pragma region Collider
Collider::Collider()
{
	m_trigger = false;
	m_visible = false;
	GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);
}
#pragma endregion Collider



#pragma region RigidBody
RigidBody::RigidBody()
{
	m_velocity.SetZero();
}

void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody