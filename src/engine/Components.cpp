#include "pch.h"
#include "Components.h"

#include "Log.h"

#include "GC.h"
#include "GameObject.h"


Component::Component()
	: m_flags(0)
{
	m_active = true;
	m_pGameObject = nullptr;

	Init();

	std::string temp = "FLAG_FIXED_UPDATE: ";
	if (IsFlagSet(FIXED_UPDATE))
		temp += "true";
	else
		temp += "false";

	temp += " | FLAG_RENDER: ";
	if (IsFlagSet(RENDER))
		temp += "true";
	else
		temp += "false";

	LogEngineDebug(temp.c_str());

	if (IsFlagSet(FIXED_UPDATE))
		;

	if (IsFlagSet(RENDER))
		;
}

#pragma region Collider
Collider::Collider()
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