#include "pch.h"
#include "Components.h"

#include "GC.h"


Component::Component()
{
	m_active = true;
	m_pGameObject = nullptr;
}

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

