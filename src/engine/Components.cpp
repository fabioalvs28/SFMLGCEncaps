#include "pch.h"
#include "Components.h"

#include "Log.h"

#include "GC.h"
#include "../../src/Render/framework.h"
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
	//GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);

}

Collider::~Collider()
{
	//GC::m_pActiveGameManager.m_pPhysicManager.UnregisterCollider(this);
	if (m_pRenderNode != nullptr)
		m_pRenderNode->Remove();
}
#pragma endregion Collider

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
#pragma region RigidBody
void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody
