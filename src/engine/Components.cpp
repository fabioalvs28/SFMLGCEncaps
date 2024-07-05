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
	pos = GCVEC2(0,0);
	m_color = GCColor();
	m_pRenderNode = nullptr;
}

SpriteRenderer::~SpriteRenderer()
{
	if (m_pRenderNode != nullptr)
		m_pRenderNode->Remove();
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

