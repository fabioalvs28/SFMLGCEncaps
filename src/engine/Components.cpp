#include "pch.h"
#include "../Render/pch.h"

using namespace DirectX;


Component::Component()
{
	m_pGameObject = nullptr;
	m_globalActive = true;
	m_selfActive = true;
	
	m_created = false;
	
	m_pUpdateNode = nullptr;
	m_pPhysicsNode = nullptr;
	m_pRenderNode = nullptr;
}



void Component::RegisterToManagers()
{
	if ( IsFlagSet( UPDATE ) )
		GC::GetActiveUpdateManager()->RegisterComponent( this );
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		GC::GetActivePhysicManager()->RegisterComponent( this );
	
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



void Component::Activate()
{
	if( m_selfActive == true )
		return;

	m_selfActive = true;

	if( m_globalActive == false )
		return;

	if( m_created == true )
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
	
	if ( m_created == false )
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
	
	if ( m_created == false )
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
	
	if ( m_created == false )
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

SpriteRenderer::SpriteRenderer()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	m_pGeometry = pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshColor(m_pGeometry).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GCShader* shaderColor = pGraphics->CreateShaderColor().resource;
	m_pMaterial = pGraphics->CreateMaterial(shaderColor).resource;
}


/////////////////////////////////////////////////
/// @brief Set Sprite of a GameObject
/// 
/// @param texturePath name of the sprite file
/// 
/// @note The sprite must be in .dds 
/////////////////////////////////////////////////

void SpriteRenderer::SetSprite(std::string fileName)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(m_pGeometry).resource;
	GCTexture* texture = pGraphics->CreateTexture( std::string("../../../src/Textures/") + fileName).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GC_RESOURCE_CREATION_RESULT<GCShader*> shaderTexture = pGraphics->CreateShaderTexture();
	GC_RESOURCE_CREATION_RESULT<GCMaterial*> mat = pGraphics->CreateMaterial(shaderTexture.resource);
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
	*(pNewComponent->m_pGeometry) = *m_pGeometry;

	return pNewComponent;
}

void SpriteRenderer::SetAnimatedSprite(GCGeometry* pGeometry, GCTexture* pTexture)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGeometry;
	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(m_pGeometry).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GC_RESOURCE_CREATION_RESULT<GCShader*> shaderTexture = pGraphics->CreateShaderTexture();
	m_pMaterial = pGraphics->CreateMaterial(shaderTexture.resource).resource;
	m_pMaterial->SetTexture(pTexture);
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

	m_pGeometry = pGraphics->CreateGeometryPrimitive(Plane, XMFLOAT4(Colors::Blue)).resource;

	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(m_pGeometry).resource;
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
	*(pNewComponent->m_pGeometry) = *m_pGeometry;
	pNewComponent->m_trigger = m_trigger;
	pNewComponent->m_visible = m_visible;

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







Camera::Camera()
{
	m_position.SetZero();
	m_target.SetZero();
	m_up.SetZero();
	
	m_viewWidth = 20.0f;
	m_viewHeight = 20.0f;
	m_nearZ = 1.0f;
	m_farZ = 1000.0f;
	
	m_viewMatrix.SetIdentity();
	m_projectionMatrix.SetIdentity();
}



void Camera::Update()
{
	bool dirty = false;
	
	if ( m_position != m_pGameObject->m_transform.m_position )
	{
		m_position = m_pGameObject->m_transform.m_position;
		dirty = true;
	}
	
	if ( m_up != m_pGameObject->m_transform.m_up )
	{
		m_up = m_pGameObject->m_transform.m_up;
		dirty = true;
	}
	
	if ( dirty == false )
		return;
	
	GC::GetActiveRenderManager()->m_pGraphics->CreateViewProjConstantBuffer( m_pGameObject->m_transform.m_position, m_target, m_pGameObject->m_transform.m_up, 0.0f, 0.0f, m_nearZ, m_farZ, m_viewWidth, m_viewHeight, GC_PROJECTION_TYPE::ORTHOGRAPHIC, m_projectionMatrix, m_viewMatrix );
}



Camera* Camera::Duplicate()
{
	Camera* pNewComponent = new Camera();

	pNewComponent->m_pGameObject = m_pGameObject; // Attention set a new gameobject after use
	pNewComponent->m_globalActive = m_globalActive;
	pNewComponent->m_selfActive = m_selfActive;

	return pNewComponent;
}


void Animator::PlayAnimation(std::string animationName)
{
	m_activeAnimation = animationName;
	m_currentAnimation = GC::GetActiveRenderManager()->GetAnimation( animationName );
}

void Animator::StopAnimation()
{
	m_activeAnimation = "";
	m_currentAnimation = nullptr;
}

void Animator::Update()
{
	if ( m_currentAnimation->Update() )
	{
		m_pGameObject->GetComponent<SpriteRenderer>()->SetAnimatedSprite(m_currentAnimation->GetGeometry(), m_currentAnimation->GetTexture());
	}
}

void Animator::LoadSpriteSheet( int row , int col , int width , int height )
{
	GCSpriteSheetGeometryLoader loader;
	m_spriteSheetInfo = loader.LoadSpriteSheet( row , col , width , height );
}

void Animator::CreateAnimation( std::string fileName, int firstFrame, int frameNumber )
{
	
}