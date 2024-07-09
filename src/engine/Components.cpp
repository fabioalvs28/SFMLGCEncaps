#include "pch.h"
#include "Components.h"

#include "Log.h"

#include "GC.h"
#include "GameObject.h"


Component::Component()
{
	m_flags = 0;
	m_active = true;
	m_pGameObject = nullptr;

	if (IsFlagSet(FIXED_UPDATE))
		;

	if (IsFlagSet(RENDER))
		;
}

Component::Component(int flags)
{
	m_flags = flags;
	m_active = true;
	m_pGameObject = nullptr;

	if (IsFlagSet(UPDATE))
		;

	if (IsFlagSet(FIXED_UPDATE))
		;

	if (IsFlagSet(RENDER))
		;
}

#pragma region Collider
Collider::Collider()
	: Component(FIXED_UPDATE | RENDER)
{
	m_trigger = false;
	m_visible = false;
	GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);
}

Collider::~Collider()
{
	GC::m_pActiveGameManager.m_pPhysicManager.UnregisterCollider(this);
}
#pragma endregion Collider

#pragma region RigidBody
void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody