#include "pch.h"
#include "Components.h"
#include "Log.h"
#include "GC.h"
#include "GameObject.h"
#include "../Render/pch.h"

using namespace DirectX;



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
	
	if ( IsFlagSet( RENDER ) )
		GC::GetActiveRenderManager()->RegisterComponent(this);
}

void Component::UnregisterFromManagers()
{
	if ( IsFlagSet( UPDATE ) )
	{
		m_pUpdateNode->Delete();
		m_pUpdateNode = nullptr;
	}
	
	if ( IsFlagSet( FIXED_UPDATE ) )
	{
		m_pPhysicsNode->Delete();
		m_pPhysicsNode = nullptr;
	}
	
	if ( IsFlagSet( RENDER ) )
	{
		m_pRenderNode->Delete();
		m_pRenderNode = nullptr;
	}
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






SpriteRenderer::SpriteRenderer()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(GC::GetActiveRenderManager()->m_pPlane).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;
}



/////////////////////////////////////////////////
/// @brief Set Sprite of a GameObject
/// 
/// @param texturePath path of the sprite file
/// 
/// @note The sprite must be in .dds 
/////////////////////////////////////////////////
void SpriteRenderer::SetSprite(std::string fileName)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(GC::GetActiveRenderManager()->m_pPlane).resource;
	GCTexture* texture = pGraphics->CreateTexture( std::string("../../../src/Textures/") + fileName).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	ResourceCreationResult<GCShader*> shaderTexture = pGraphics->CreateShaderTexture();
	ResourceCreationResult<GCMaterial*> mat = pGraphics->CreateMaterial(shaderTexture.resource);
	m_pMaterial = mat.resource;
	m_pMaterial->SetTexture(texture);

	
}

SpriteRenderer* SpriteRenderer::Duplicate()
{
	SpriteRenderer* pNewComponent = new SpriteRenderer();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	pNewComponent->m_color = m_color; 
	*(pNewComponent->m_pMesh) = *m_pMesh;
	*(pNewComponent->m_pMaterial) = *m_pMaterial;
	
	return pNewComponent;
}


void SpriteRenderer::Render()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, m_pGameObject->m_transform.m_worldMatrix);
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

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






BoxCollider::BoxCollider()
{

	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(GC::GetActiveRenderManager()->m_pPlane).resource;
	GCTexture* texture = pGraphics->CreateTexture("../../../src/Textures/BoxColliderSquare.dds").resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	auto shaderTexture = pGraphics->CreateShaderTexture();
	auto mat = pGraphics->CreateMaterial(shaderTexture.resource);
	m_pMaterial = mat.resource;
	m_pMaterial->SetTexture(texture);

}

BoxCollider* BoxCollider::Duplicate()
{
	BoxCollider* pNewComponent = new BoxCollider();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	*(pNewComponent->m_pMesh) = *m_pMesh;
	*(pNewComponent->m_pMaterial) = *m_pMaterial;
	pNewComponent->m_trigger = m_trigger;
	pNewComponent->m_visible = m_visible;
	pNewComponent->m_size = m_size;

	return pNewComponent;
}

void BoxCollider::Render()
{
	if (m_visible == false)
		return;

	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, m_pGameObject->m_transform.m_worldMatrix);
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

}



CircleCollider* CircleCollider::Duplicate()
{
	CircleCollider* pNewComponent = new CircleCollider();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	*(pNewComponent->m_pMesh) = *m_pMesh;
	*(pNewComponent->m_pMaterial) = *m_pMaterial;
	pNewComponent->m_trigger = m_trigger;
	pNewComponent->m_visible = m_visible;
	pNewComponent->m_radius = m_radius;

	return pNewComponent;
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



RigidBody* RigidBody::Duplicate()
{
	RigidBody* pNewComponent = new RigidBody();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	pNewComponent->m_velocity = m_velocity;

	return pNewComponent;
}



Animator* Animator::Duplicate()
{
	Animator* pNewComponent = new Animator();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	return pNewComponent;
}



SoundMixer* SoundMixer::Duplicate()
{
	SoundMixer* pNewComponent = new SoundMixer();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	return pNewComponent;
}


Camera* Camera::Duplicate()
{
	Camera* pNewComponent = new Camera();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	return pNewComponent;
}