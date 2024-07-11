#include "pch.h"
#include "Components.h"

#include "GC.h"
#include "../../src/Render/framework.h"


Component::Component()
{
	m_active = true;
	m_pGameObject = nullptr;
}

SpriteRenderer::SpriteRenderer()
{
	GC::m_pActiveGameManager.m_pRenderManager.RegisterComponent(this);

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;


	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMesh(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane);
	pGraphics->InitializeGraphicsResourcesEnd();


	GCShader* shaderColor = pGraphics->CreateShaderColor();
	m_pMaterial = pGraphics->CreateMaterial(shaderColor);

	m_color = GCColor();
}

SpriteRenderer::~SpriteRenderer()
{
	if (m_pRenderNode != nullptr)
		m_pRenderNode->Remove();
}

void SpriteRenderer::Render()
{
}

void SpriteRenderer::SetSprite(std::string texturePath)
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	GCTexture* texture = pGraphics->CreateTexture(texturePath);
	pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* shaderTexture = pGraphics->CreateShaderTexture();
	GCMaterial* pMaterial = pGraphics->CreateMaterial(shaderTexture);
	m_pMaterial = pMaterial;
	m_pMaterial->SetTexture(texture);
}

void SpriteRenderer::SetColor()
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	GCShader* shaderColor = pGraphics->CreateShaderColor();
	m_pMaterial = pGraphics->CreateMaterial(shaderColor);
}

Collider::Collider()
{
    m_trigger = false;
	m_visible = false;
	//GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);

}

Collider::~Collider()
{
	//GC::m_pActiveGameManager.m_pPhysicManager.UnregisterCollider(this);
	if (m_pRenderNode != nullptr)
		m_pRenderNode->Remove();
}

BoxCollider::BoxCollider()
{
	GC::m_pActiveGameManager.m_pRenderManager.RegisterComponent(this);

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMesh(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane);
	pGraphics->InitializeGraphicsResourcesEnd();


	GCShader* shaderColor = pGraphics->CreateShaderColor();
	m_pMaterial = pGraphics->CreateMaterial(shaderColor);

}

CircleCollider::CircleCollider()
{
	GC::m_pActiveGameManager.m_pRenderManager.RegisterComponent(this);

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMesh(GC::m_pActiveGameManager.m_pRenderManager.m_pCircle);
	pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* shaderColor = pGraphics->CreateShaderColor();
	m_pMaterial = pGraphics->CreateMaterial(shaderColor);
}