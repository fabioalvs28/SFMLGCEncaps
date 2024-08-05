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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Copies the values of this Component into the given Component.
/// 
/// @param pNewComponent The Component that will receive the values.
/// 
/// @note The m_pGameObject won't be passed to the new Component as a GameObject can't have more than one Component of the same ID.
/// @note The m_globalActive won't be passed to the new Component as it doesn't have any GameObject.
/// @note The new Component won't be registered to the Managers as it will be registered the next frame.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Component::CopyTo( Component* pDestination )
{ pDestination->m_selfActive = m_selfActive; }



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
	OnActivate();
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
	OnDeactivate();
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
	OnActivate();
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
	OnDeactivate();
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
	m_pMesh = pGraphics->CreateMeshTexture(m_pGeometry).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	GC_RESOURCE_CREATION_RESULT<GCShader*> shaderTexture = pGraphics->CreateShaderTexture();
	GC_RESOURCE_CREATION_RESULT<GCMaterial*> mat = pGraphics->CreateMaterial( shaderTexture.resource );
	m_pMaterial = mat.resource;
}



void SpriteRenderer::CopyTo( Component* pDestination )
{
	Component::CopyTo( pDestination );
	SpriteRenderer* pSpriteRenderer = static_cast<SpriteRenderer*>( pDestination );
	*(pSpriteRenderer->m_pMesh) = *m_pMesh; //! Need to ask Render if this will work
	*(pSpriteRenderer->m_pMaterial) = *m_pMaterial; //! Need to ask Render if this will work
}

void SpriteRenderer::Render()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, m_pGameObject->m_transform.GetWorldMatrix());
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

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

	m_pMaterial->SetTexture(texture);
}


void SpriteRenderer::SetAnimatedSprite(GCGeometry* pGeometry, GCTexture* pTexture)
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pGeometry = pGeometry;
	pGraphics->InitializeGraphicsResourcesStart();
	m_pMesh = pGraphics->CreateMeshTexture(m_pGeometry).resource;
	pGraphics->InitializeGraphicsResourcesEnd();

	m_pMaterial->SetTexture(pTexture);
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



void Collider::CopyTo( Component* pDestination )
{
	Component::CopyTo( pDestination );
	Collider* pCollider = static_cast<Collider*>( pDestination );
	pCollider->m_trigger = m_trigger;
	pCollider->m_visible = m_visible;
	*(pCollider->m_pMesh) = *m_pMesh; //! Need to ask Render if this will work
	*(pCollider->m_pMaterial) = *m_pMaterial; //! Need to ask Render if this will work
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



void BoxCollider::CopyTo( Component* pDestination )
{ Collider::CopyTo( pDestination ); }

void BoxCollider::Render()
{
	if (m_visible == false)
		return;

	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;

	pGraphics->UpdateWorldConstantBuffer(m_pMaterial, m_pGameObject->m_transform.GetWorldMatrix());
	pGraphics->GetRender()->DrawObject(m_pMesh, m_pMaterial, true);

}






void CircleCollider::CopyTo( Component* pDestination )
{ Collider::CopyTo( pDestination ); }






RigidBody::RigidBody()
{
	m_velocity.SetZero();
}



void RigidBody::CopyTo( Component* pDestination )
{
	Component::CopyTo( pDestination );
	RigidBody* pRigidBody = static_cast<RigidBody*>( pDestination );
	pRigidBody->m_velocity = m_velocity;
}

void RigidBody::FixedUpdate()
{
	// Apply velocity
	m_pGameObject->m_transform.Translate(m_velocity);		// TODO: Multiply by the fixed delta time
}






void SoundMixer::CopyTo( Component* pDestination )
{ Component::CopyTo( pDestination ); }






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



void Camera::CopyTo( Component* pDestination )
{
	Component::CopyTo( pDestination );
	Camera* pCamera = static_cast<Camera*>( pDestination );
	pCamera->m_nearZ = m_nearZ;
    pCamera->m_farZ = m_farZ;
    pCamera->m_viewWidth = m_viewWidth;
    pCamera->m_viewHeight = m_viewHeight;
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





Animator::Animator()
{
	m_currentAnimation = nullptr;
	m_activeAnimationName = "";
	m_spritesheetName = "";
	m_pSpriteRenderer = nullptr;
	m_pSpriteSheetInfo = nullptr;
	m_currentFrameIndex = 0;
	m_currentFrameTime = 0.0f;
}



void Animator::CopyTo( Component* pDestination )
{
	Component::CopyTo( pDestination );
	Animator* pAnimator = static_cast<Animator*>( pDestination );
	// todo
}

void Animator::Start()
{
	SpriteRenderer* pSpriteRenderer = m_pGameObject->GetComponent<SpriteRenderer>();
	ASSERT( pSpriteRenderer != nullptr , LOG_FATAL , "Trying to add Animator without a SpriteRenderer" );
	m_pSpriteRenderer = pSpriteRenderer;
}

void Animator::Update()
{
	if ( m_currentAnimation == nullptr )
		return;

	if ( m_currentAnimation->Update( &m_currentFrameIndex, &m_currentFrameTime ) )
	{
		m_pSpriteRenderer->SetAnimatedSprite(m_currentAnimation->GetGeometry(), m_currentAnimation->GetTexture());
	}
}


/////////////////////////////////////////////////
/// @brief Start the choosen animation
/// 
/// @param animationName Animation's Name
/////////////////////////////////////////////////
void Animator::PlayAnimation(std::string animationName)
{
	if ( m_activeAnimationName == animationName )
		return;
	Animation* pAnimation = GC::GetActiveRenderManager()->GetAnimation( animationName );
	ASSERT( pAnimation != nullptr , LOG_FATAL , "Trying to play a non-existent animation" );
	m_activeAnimationName = animationName;
	m_currentAnimation = pAnimation;
	m_currentAnimation->StartAnimation();
	m_pSpriteRenderer->SetAnimatedSprite( m_currentAnimation->GetGeometry() , m_currentAnimation->GetTexture() );
}

/////////////////////////////////////////////////////////////
/// @brief Stop the current animation and keep last sprite
/////////////////////////////////////////////////////////////
void Animator::StopAnimation()
{
	m_activeAnimationName = "";
	m_currentAnimation = nullptr;
}

///////////////////////////////////////////////////////
/// @brief Load a SpriteSheet in the animator
///
/// @param fileName SpriteSheet 's name with .dds 
/// @param row row number in the spritesheet
/// @param col column number in the spritesheet
/// @param width spritesheet's width in pixel
/// @param height spritesheet's height in pixel
/// 
/// @note An animator can load only one spritesheet.
///////////////////////////////////////////////////////
void Animator::LoadSpriteSheet(std::string fileName, int row , int col , int width , int height )
{
	m_spritesheetName = fileName;
	GCSpriteSheetGeometryLoader loader;

	m_pSpriteSheetInfo = new GC_SPRITESHEET_INFO(loader.LoadSpriteSheet( row , col , width , height ));
}


//////////////////////////////////////////////////////////////////////////////////
/// @brief Create an Animation
///
/// @param animationName animation's name
/// @param firstFrame the first frame id of the animation in the spritesheet 
/// @param frameNumber the number of frame in the spritesheet for the animation
/// @param frameDisplayTime How long each frame will be display
//////////////////////////////////////////////////////////////////////////////////
Animation* Animator::CreateAnimation( std::string animationName, int firstFrame, int frameNumber, float frameDisplayTime )
{
	ASSERT( m_pSpriteSheetInfo != nullptr, LOG_FATAL , "Trying to create an animation without any Spritesheet loaded" );
	ASSERT( GC::GetActiveRenderManager()->GetAnimation( animationName ) == nullptr , LOG_FATAL , "Trying to create a new animation with an existent animation's name" );
	Animation* pNewAnimation = new Animation();
	pNewAnimation->SetSpriteSheet( m_spritesheetName , m_pSpriteSheetInfo );
	for ( int i = firstFrame; i < firstFrame + frameNumber; i++ )
	{
		pNewAnimation->AddFrame( i, frameDisplayTime );
	}
	GC::GetActiveRenderManager()->AddAnimation( pNewAnimation , animationName );

	return pNewAnimation;
}


/////////////////////////////////////////////////////////////////////////////
/// @brief Create an Animation if you need to select all the frame
///
/// @param animationName animation's name
/// @param frameList the frame id list of the animation in the spritesheet
/// @param frameDisplayTime How long each frame will be display
/////////////////////////////////////////////////////////////////////////////
Animation* Animator::CreateAnimationWithCustomFrames( std::string animationName , std::vector<int> frameList, float frameDisplayTime )
{
	ASSERT( m_pSpriteSheetInfo != nullptr , LOG_FATAL , "Trying to create an animation without any Spritesheet loaded" );
	ASSERT( GC::GetActiveRenderManager()->GetAnimation( animationName ) == nullptr , LOG_FATAL , "Trying to create a new animation with an existent animation's name" );
	Animation* pNewAnimation = new Animation();
	pNewAnimation->SetSpriteSheet( m_spritesheetName , m_pSpriteSheetInfo );

	for ( int i = 0; i < frameList.size() ; i++ )
	{
		pNewAnimation->AddFrame( frameList[i] , frameDisplayTime );
	}
	GC::GetActiveRenderManager()->AddAnimation( pNewAnimation , animationName );

	return pNewAnimation;
}

