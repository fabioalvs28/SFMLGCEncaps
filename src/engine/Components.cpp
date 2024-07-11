#include "Components.h"

#include "Log.h"

#include "GC.h"
#include "../../src/Render/pch.h"
#include "GameObject.h"



Component::Component() { Init(); }

Component::Component( int flags )
{
    Init();
    m_flags = flags;
    
    if ( IsFlagSet( UPDATE ) )
        ;

    if ( IsFlagSet( FIXED_UPDATE ) )
        ;

    if ( IsFlagSet( RENDER ) )
			GC::m_pActiveGameManager.m_pRenderManager.RegisterComponent(this);
        ;
}

void Component::Init()
{
    m_flags = 0;
    m_active = true;
    m_pGameObject = nullptr;
    
    m_pUpdateNode = nullptr;
    m_pPhysicsNode = nullptr;
    m_pRenderNode = nullptr;
}



SpriteRenderer::SpriteRenderer() : Component(RENDER)
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
	pGraphics->InitializeGraphicsResourcesEnd();


	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;

	m_color = GCColor();
}

SpriteRenderer::~SpriteRenderer()
{
	if (m_pRenderNode != nullptr)
		m_pRenderNode->Remove();
}


void SpriteRenderer::SetSprite(std::string texturePath)
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	GCTexture* texture = pGraphics->CreateTexture(texturePath).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* shaderTexture = pGraphics->CreateShaderTexture().resource;
	GCMaterial* pMaterial = pGraphics->CreateMaterial(shaderTexture).resource;
	m_pMaterial = pMaterial;
	m_pMaterial->SetTexture(texture);
}

void SpriteRenderer::SetColor()
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;
}



#pragma region Collider
Collider::Collider() : Component( FIXED_UPDATE | RENDER )
{
	m_trigger = false;
	m_visible = false;
	//GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);

}

#pragma endregion Collider

BoxCollider::BoxCollider()
{

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;


	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
	pGraphics->InitializeGraphicsResourcesEnd();


	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;

}

CircleCollider::CircleCollider()
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;


	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
	pGraphics->InitializeGraphicsResourcesEnd();


	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;
}



#pragma region RigidBody
void RigidBody::FixedUpdate()
{
    // Apply velocity
    m_pGameObject->m_transform.Translate(m_velocity);        // TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody