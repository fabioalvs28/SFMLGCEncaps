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
		GC::GetActiveUpdateManager()->RegisterComponent( this );
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		GC::GetActiveUpdateManager()->RegisterComponent( this );
	
	// if ( IsFlagSet( RENDER ) )
	// 	Gc::GetActiveRenderManager()->RegisterComponent( this );
}

void Component::UnregisterFromManagers()
{
	if ( IsFlagSet( UPDATE ) )
		m_pUpdateNode->Delete();
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		m_pPhysicsNode->Delete();
	
	if ( IsFlagSet( RENDER ) )
		m_pRenderNode->Delete();
}



void Component::SetActive( bool active )
{
	m_active = active;
	if ( active == true )
		RegisterToManagers();
	else
	    UnregisterFromManagers();
}



Collider::Collider()
{
	m_trigger = false;
	m_visible = false;
}

void Collider::RegisterToManagers()
{
	Component::RegisterToManagers();
	GC::GetActivePhysicManager()->RegisterCollider( this );
}

void Collider::UnregisterFromManagers()
{
	Component::UnregisterFromManagers();
	m_pColliderNode->Delete();
}



RigidBody::RigidBody()
{
	m_velocity.SetZero();
}

void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}