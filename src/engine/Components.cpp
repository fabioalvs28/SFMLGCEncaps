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
	 	GC::GetActiveRenderManager()->RegisterComponent( this );
}

void Component::UnregisterFromManagers()
{
	if ( IsFlagSet( UPDATE ) )
		m_pUpdateNode->Delete();
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		m_pPhysicsNode->Delete();
	
	if ( IsFlagSet( RENDER ) )
		m_pRenderNode->Delete();
}


SpriteRenderer::SpriteRenderer(GCGameObject* pGameObject) : Component(pGameObject)
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
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
void SpriteRenderer::SetSprite(std::string texturePath)
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
	GCTexture* texture = pGraphics->CreateTexture(texturePath).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	auto shaderTexture = pGraphics->CreateShaderTexture();
	auto mat = pGraphics->CreateMaterial(shaderTexture.resource);
	m_pMaterial = mat.resource;
	m_pMaterial->SetTexture(texture);

	
}


void SpriteRenderer::Render()
{
	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	XMMATRIX worldMatrix = XMMatrixScaling(m_pGameObject->m_transform.m_scale.x, m_pGameObject->m_transform.m_scale.y, m_pGameObject->m_transform.m_scale.z) * XMMatrixTranslation(m_pGameObject->m_transform.m_position.x, m_pGameObject->m_transform.m_position.y, m_pGameObject->m_transform.m_position.z); 

	XMFLOAT4X4 worldMatrix4X4;
	XMStoreFloat4x4(&worldMatrix4X4, worldMatrix);

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, worldMatrix4X4);
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

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

BoxCollider::BoxCollider(GCGameObject* pGameObject) : Collider(pGameObject)
{

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(GC::m_pActiveGameManager.m_pRenderManager.m_pPlane).resource;
	GCTexture* texture = pGraphics->CreateTexture("C:/Users/emaillard-salin/Desktop/BoxColliderSquare.dds").resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	auto shaderTexture = pGraphics->CreateShaderTexture();
	auto mat = pGraphics->CreateMaterial(shaderTexture.resource);
	m_pMaterial = mat.resource;
	m_pMaterial->SetTexture(texture);

}

void BoxCollider::Render()
{

	if (m_visible == false)
		return;

	GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	XMMATRIX meshMatrix = XMMatrixScaling(m_pGameObject->m_transform.m_scale.x + m_size.x , m_pGameObject->m_transform.m_scale.y + m_size.y , m_pGameObject->m_transform.m_scale.z) * XMMatrixTranslation(m_pGameObject->m_transform.m_position.x, m_pGameObject->m_transform.m_position.y, m_pGameObject->m_transform.m_position.z);

	XMFLOAT4X4 meshMatrix4X4;
	XMStoreFloat4x4(&meshMatrix4X4, meshMatrix);

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, meshMatrix4X4);
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

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
