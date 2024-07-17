#include "pch.h"
#include "Components.h"

#include "Log.h"
#include "GameObject.h"
#include "GC.h"



Component::Component()
{
	m_globalActive = true;
	m_selfActive = true;
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
	if ( IsActive() == true )
	{
		if ( active == false )
		{
			m_selfActive = active;
	    	UnregisterFromManagers();
		}
	}
	else
	{
		m_selfActive = active;
		if ( IsActive() == true )
			RegisterToManagers();
	}
}

void Component::SetGlobalActive( bool active )
{
	if ( IsActive() == true )
	{
		if ( active == false )
		{
			m_globalActive = active;
	    	UnregisterFromManagers();
		}
	}
	else
	{
		m_globalActive = active;
		if ( IsActive() == true )
			RegisterToManagers();
	}
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