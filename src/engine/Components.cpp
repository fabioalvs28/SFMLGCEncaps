#include "pch.h"
#include "Components.h"
#include "Log.h"
#include "GC.h"
#include "GameObject.h"
#include "../Render/pch.h"


using namespace DirectX;


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

#pragma region Collider
Collider::Collider() : Component( FIXED_UPDATE | RENDER )
{
	m_trigger = false;
	m_visible = false;
	GC::m_pActiveGameManager.m_pPhysicManager.RegisterCollider(this);

}

#pragma endregion Collider

BoxCollider::BoxCollider()
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


CircleCollider::CircleCollider()
{
	//if (m_visible == false)
	//	return;

	//GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;


	//pGraphics->InitializeGraphicsResourcesStart();
	//m_pMesh = pGraphics->CreateMeshColor(GC::m_pActiveGameManager.m_pRenderManager.m_pCircle).resource;
	//pGraphics->InitializeGraphicsResourcesEnd();


	//GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	//m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;
}

void CircleCollider::Render()
{
	//GCGraphics* pGraphics = GC::m_pActiveGameManager.m_pRenderManager.m_pGraphics;

	//XMMATRIX worldMatrix = XMMatrixScaling(m_pGameObject->m_transform.m_scale.x, m_pGameObject->m_transform.m_scale.y, m_pGameObject->m_transform.m_scale.z) * XMMatrixTranslation(m_pGameObject->m_transform.m_position.x, m_pGameObject->m_transform.m_position.y, m_pGameObject->m_transform.m_position.z);

	//XMFLOAT4X4 worldMatrix4X4;
	//XMStoreFloat4x4(&worldMatrix4X4, worldMatrix);

	//pGraphics->UpdateWorldConstantBuffer(m_pMaterial, worldMatrix4X4);
	//pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);
}

#pragma region RigidBody
void RigidBody::FixedUpdate()
{
    // Apply velocity
    m_pGameObject->m_transform.Translate(m_velocity);        // TODO: Multiply by the fixed delta time
}
#pragma endregion RigidBody