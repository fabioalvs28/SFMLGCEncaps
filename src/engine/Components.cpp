#include "pch.h"
#include "Components.h"


Component::Component()
{
	m_active = true;
	m_pGameObject = nullptr;
}

Collider::Collider()
{
    m_trigger = false;
}

