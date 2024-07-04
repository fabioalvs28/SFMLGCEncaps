#include "pch.h"
#include "Components.h"

#include "GC.h"


Component::Component()
{
	m_active = true;
	m_pGameObject = nullptr;
}

SpriteRenderer::SpriteRenderer()
{
	GC::m_pActiveGameManager.m_pRenderManager.RegisterSpriteRenderer(this);
}

void SpriteRenderer::SetSprite(std::string texturePath)
{
	GC::m_pActiveGameManager.m_pRenderManager.SetShaderTexture(this, texturePath);
}

void SpriteRenderer::SetColor()
{
	GC::m_pActiveGameManager.m_pRenderManager.SetShaderColor(this);
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

