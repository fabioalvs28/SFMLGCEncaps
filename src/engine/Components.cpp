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



void Component::Activate()
{
	if( m_selfActive == true )
		return;

	m_selfActive = true;

	if( m_globalActive == false )
		return;

	if( IsCreated() == true )
		return;

	RegisterToManagers();
}

void Component::Deactivate()
{
	if ( m_selfActive == false )
		return;
	
	m_selfActive = false;
	
	if ( m_globalActive == false )
	    return;
	
	if ( IsCreated() == false )
	    return;
	
	UnregisterFromManagers();
}

void Component::ActivateGlobal()
{
	if ( m_globalActive == true )
		return;
	
	m_globalActive = true;
	
	if ( m_selfActive == false )
		return;
	
	if ( IsCreated() == false )
		return;
	
	RegisterToManagers();
}

void Component::DeactivateGlobal()
{
	if ( m_globalActive == false )
		return;
	
	m_globalActive = false;
	
	if ( m_selfActive == false )
		return;
	
	if ( IsCreated() == false )
		return;
	
	UnregisterFromManagers();
}

void Component::SetActive( bool active )
{
	if ( active == true )
	{
		Activate();
		return;
	}
	Deactivate();
}

bool Component::IsCreated()
{
	if ( m_pGameObject == nullptr )
		return true;
	
	return m_pGameObject->m_created;
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
void SpriteRenderer::SetSprite(std::string texturePath)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(GC::GetActiveRenderManager()->m_pPlane).resource;
	GCTexture* texture = pGraphics->CreateTexture( std::string("../../../src/Textures/") + texturePath).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	ResourceCreationResult<GCShader*> shaderTexture = pGraphics->CreateShaderTexture();
	ResourceCreationResult<GCMaterial*> mat = pGraphics->CreateMaterial(shaderTexture.resource);
	m_pMaterial = mat.resource;
	m_pMaterial->SetTexture(texture);

	
}



void SpriteRenderer::Render()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	XMFLOAT4X4 worldMatrix;
	
	worldMatrix._11 = m_pGameObject->m_transform.m_worldMatrix._11;
	worldMatrix._12 = m_pGameObject->m_transform.m_worldMatrix._12;
	worldMatrix._13 = m_pGameObject->m_transform.m_worldMatrix._13;
	worldMatrix._14 = m_pGameObject->m_transform.m_worldMatrix._14;
	
	worldMatrix._21 = m_pGameObject->m_transform.m_worldMatrix._21;
	worldMatrix._22 = m_pGameObject->m_transform.m_worldMatrix._22;
	worldMatrix._23 = m_pGameObject->m_transform.m_worldMatrix._23;
	worldMatrix._24 = m_pGameObject->m_transform.m_worldMatrix._24;
	
	worldMatrix._31 = m_pGameObject->m_transform.m_worldMatrix._31;
	worldMatrix._32 = m_pGameObject->m_transform.m_worldMatrix._32;
	worldMatrix._33 = m_pGameObject->m_transform.m_worldMatrix._33;
	worldMatrix._34 = m_pGameObject->m_transform.m_worldMatrix._34;
	
	worldMatrix._41 = m_pGameObject->m_transform.m_worldMatrix._41;
	worldMatrix._42 = m_pGameObject->m_transform.m_worldMatrix._42;
	worldMatrix._43 = m_pGameObject->m_transform.m_worldMatrix._43;
	worldMatrix._44 = m_pGameObject->m_transform.m_worldMatrix._44;

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, worldMatrix);
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



void BoxCollider::Render()
{
	if (m_visible == false)
		return;

	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

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
