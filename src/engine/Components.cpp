#include "pch.h"

using namespace DirectX;


GCComponent::GCComponent()
{
	m_pGameObject = nullptr;
	m_globalActive = true;
	m_selfActive = true;
	
	m_created = false;
	m_registered = false;
	
	m_pUpdateNode = nullptr;
	m_pPhysicsNode = nullptr;
	m_pRenderNode = nullptr;
}


void GCComponent::Destroy()
{
	m_pGameObject = nullptr;

	if ( m_pUpdateNode != nullptr )
		m_pUpdateNode->Delete();
	if ( m_pPhysicsNode != nullptr )
		m_pPhysicsNode->Delete();
	if ( m_pRenderNode != nullptr )
		m_pRenderNode->Delete();
}

void GCComponent::RegisterToManagers()
{
	if ( IsFlagSet( UPDATE ) )
		GC::GetActiveUpdateManager()->RegisterComponent( this );
	
	if ( IsFlagSet( FIXED_UPDATE ) )
		GC::GetActivePhysicManager()->RegisterComponent( this );
	
	if ( IsFlagSet( RENDER ) )
		GC::GetActiveRenderManager()->RegisterComponent( this );

	m_registered = true;
}

void GCComponent::UnregisterFromManagers()
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

	m_registered = false;
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
void GCComponent::CopyTo( GCComponent* pDestination )
{ pDestination->m_selfActive = m_selfActive; }



void GCComponent::Activate()
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

void GCComponent::Deactivate()
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

void GCComponent::ActivateGlobal()
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

void GCComponent::DeactivateGlobal()
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

void GCComponent::SetActive( bool active )
{
	if ( active == true )
	{
		Activate();
		return;
	}
	Deactivate();
}






GCSpriteRenderer::GCSpriteRenderer()
{
	m_pSprite = nullptr; m_isFlippedX = false;  m_isFlippedY = false;
}


GCSpriteRenderer::~GCSpriteRenderer()
{ m_pSprite = nullptr; }

void GCSpriteRenderer::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCSpriteRenderer* pSpriteRenderer = static_cast<GCSpriteRenderer*>( pDestination );
	pSpriteRenderer->m_pSprite = m_pSprite;
}

void GCSpriteRenderer::Render()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	pGraphics->UpdateWorldConstantBuffer( m_pSprite->m_pMaterial, m_pGameObject->m_transform.GetWorldMatrix() );
	pGraphics->GetRender()->DrawObject( m_pSprite->m_pMesh, m_pSprite->m_pMaterial, true );
}



void GCSpriteRenderer::SetSprite( GCSprite* pSprite )
{
	if ( m_pSprite == nullptr )
	{
		m_pSprite = pSprite;
		return;
	}
	m_pSprite->m_pGeometry = pSprite->m_pGeometry;
	m_pSprite->m_pMaterial = pSprite->m_pMaterial;
	m_pSprite->m_pMesh = pSprite->m_pMesh;
}

void GCSpriteRenderer::SetAnimatedSprite( GCGeometry* pGeometry )
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	m_pSprite->m_pGeometry = pGeometry;
	
	pGraphics->InitializeGraphicsResourcesStart();
	m_pSprite->m_pMesh = pGraphics->CreateMeshTexture( m_pSprite->m_pGeometry ).resource;
	pGraphics->InitializeGraphicsResourcesEnd();
}

void GCSpriteRenderer::FlipX()
{

	DirectX::XMFLOAT2 uv0 = m_pSprite->m_pGeometry->uv[0];
	DirectX::XMFLOAT2 uv1 = m_pSprite->m_pGeometry->uv[1];

	m_pSprite->m_pGeometry->uv[0] = m_pSprite->m_pGeometry->uv[2];
	m_pSprite->m_pGeometry->uv[2] = uv0;
	m_pSprite->m_pGeometry->uv[1] = m_pSprite->m_pGeometry->uv[3];
	m_pSprite->m_pGeometry->uv[3] = uv1;

	//GC::GetActiveRenderManager()->m_pGraphics->
	m_pSprite->m_pMesh->UpdateGeometryData();
}

void GCSpriteRenderer::FlipY()
{

	DirectX::XMFLOAT2 uv0 = m_pSprite->m_pGeometry->uv[0];
	DirectX::XMFLOAT2 uv2 = m_pSprite->m_pGeometry->uv[2];

	m_pSprite->m_pGeometry->uv[0] = m_pSprite->m_pGeometry->uv[1];
	m_pSprite->m_pGeometry->uv[1] = uv0;
	m_pSprite->m_pGeometry->uv[2] = m_pSprite->m_pGeometry->uv[3];
	m_pSprite->m_pGeometry->uv[3] = uv2;

	m_pSprite->m_pMesh->UpdateGeometryData();
}


GCCollider::GCCollider()
{
	m_trigger = false;
	m_visible = false;
}



void GCCollider::RegisterToManagers()
{
	GCComponent::RegisterToManagers();
	GC::GetActivePhysicManager()->RegisterCollider( this );
}

void GCCollider::UnregisterFromManagers()
{
	GCComponent::UnregisterFromManagers();
	m_pColliderNode->Delete();
}



void GCCollider::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCCollider* pCollider = static_cast<GCCollider*>( pDestination );
	pCollider->m_trigger = m_trigger;
	pCollider->m_visible = m_visible;
}






GCBoxCollider::GCBoxCollider() {}



void GCBoxCollider::CopyTo( GCComponent* pDestination )
{ GCCollider::CopyTo( pDestination ); }

void GCBoxCollider::Render()
{
	if ( m_visible == false )
		return;

	GCRenderManager* pRenderManager = GC::GetActiveRenderManager();
	GCGraphics* pGraphics = pRenderManager->m_pGraphics;

	GCSprite* pSprite = pRenderManager->m_pSpriteCollider;
	pGraphics->UpdateWorldConstantBuffer( pSprite->m_pMaterial, m_pGameObject->m_transform.GetWorldMatrix() );
	pGraphics->GetRender()->DrawObject( pSprite->m_pMesh, pSprite->m_pMaterial, true );
	
}






void GCCircleCollider::CopyTo( GCComponent* pDestination )
{ GCCollider::CopyTo( pDestination ); }






GCRigidBody::GCRigidBody()
{ m_velocity.SetZero(); }



void GCRigidBody::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCRigidBody* pRigidBody = static_cast<GCRigidBody*>( pDestination );
	pRigidBody->m_velocity = m_velocity;
}

void GCRigidBody::Update()
{ m_pGameObject->m_transform.Translate( m_velocity * GC::GetActiveTimer()->DeltaTime() ); }





GCAnimator::GCAnimator()
{
	m_pCurrentAnimation = nullptr;
	m_activeAnimationName = "";
	m_spritesheetName = "";
	m_pSpriteRenderer = nullptr;
	m_spriteSheetID = -1;
	m_currentFrameIndex = 0;
	m_currentFrameTime = 0.0f;
}


GCAnimator::~GCAnimator()
{
	m_pCurrentAnimation = nullptr;
	m_pSpriteRenderer = nullptr;
}

void GCAnimator::Start()
{
	GCSpriteRenderer* pSpriteRenderer = m_pGameObject->GetComponent<GCSpriteRenderer>();
	ASSERT( pSpriteRenderer != nullptr , LOG_FATAL , "Trying to add Animator without a SpriteRenderer" );
	m_pSpriteRenderer = pSpriteRenderer;
}

void GCAnimator::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCAnimator* pAnimator = static_cast<GCAnimator*>( pDestination );
	
	pAnimator->m_spritesheetName = m_spritesheetName;
	
	pAnimator->m_activeAnimationName = m_activeAnimationName;
	pAnimator->m_pCurrentAnimation = m_pCurrentAnimation;
	
	pAnimator->m_isLoop = m_isLoop;
	pAnimator->m_lastFrameIndex = m_lastFrameIndex;
	pAnimator->m_currentFrameIndex = m_currentFrameIndex;
	pAnimator->m_currentFrameTime = m_currentFrameTime;
}

void GCAnimator::Update()
{
	if ( m_pCurrentAnimation == nullptr )
		return;

	if ( m_isLoop == false && m_currentFrameIndex == m_pCurrentAnimation->GetLastFrameIndex() )
	{
		StopAnimation();
		return;
	}

	if ( m_pCurrentAnimation->Update( &m_currentFrameIndex, &m_currentFrameTime ) )
		m_pSpriteRenderer->SetAnimatedSprite( m_pCurrentAnimation->GetGeometry() );
}


/////////////////////////////////////////////////
/// @brief Start the choosen animation
/// 
/// @param animationName Animation's Name
/////////////////////////////////////////////////
void GCAnimator::PlayAnimation( std::string animationName, bool isLoop )
{
	if ( m_activeAnimationName == animationName )
		return;
	GCAnimation* pAnimation = GC::GetActiveRenderManager()->GetAnimation( animationName );
	ASSERT( pAnimation != nullptr , LOG_FATAL , "Trying to play a non-existent animation" );
	m_isLoop = isLoop;
	m_activeAnimationName = animationName;
	m_pCurrentAnimation = pAnimation;
	m_lastFrameIndex = pAnimation->GetLastFrameIndex();
	m_pCurrentAnimation->StartAnimation();
	m_pSpriteRenderer->m_pSprite->m_pMaterial = m_pCurrentAnimation->m_pMaterial;
	m_pSpriteRenderer->SetAnimatedSprite( m_pCurrentAnimation->GetGeometry() );
}

/////////////////////////////////////////////////////////////
/// @brief Stop the current animation and keep last sprite
/////////////////////////////////////////////////////////////
void GCAnimator::StopAnimation()
{
	m_activeAnimationName = "";
	m_pCurrentAnimation = nullptr;
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
void GCAnimator::LoadSpriteSheet(std::string fileName, int spriteSheetID)
{
	m_spritesheetName = fileName;
	m_spriteSheetID = spriteSheetID;
}


//////////////////////////////////////////////////////////////////////////////////
/// @brief Create an Animation
///
/// @param animationName animation's name
/// @param firstFrame the first frame id of the animation in the spritesheet 
/// @param frameNumber the number of frame in the spritesheet for the animation
/// @param frameDisplayTime How long each frame will be display
/// !!! Need to use 1 out of 2 frame because spritesheet generator create an empty frame every 1 out of 2 frame
//////////////////////////////////////////////////////////////////////////////////
GCAnimation* GCAnimator::CreateAnimation( std::string animationName, int firstFrame, int frameNumber, float frameDisplayTime )
{
	ASSERT( m_spriteSheetID != -1, LOG_FATAL , "Trying to create an animation without any Spritesheet loaded" );
	ASSERT( GC::GetActiveRenderManager()->GetAnimation( animationName ) == nullptr , LOG_FATAL , "Trying to create a new animation with an existent animation's name" );
	GCAnimation* pNewAnimation = new GCAnimation();
	pNewAnimation->SetSpriteSheet( m_spritesheetName , m_spriteSheetID);
	for ( int i = firstFrame; i < firstFrame + frameNumber; i ++ )
		pNewAnimation->AddFrame( i, frameDisplayTime );
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
GCAnimation* GCAnimator::CreateAnimationWithCustomFrames( std::string animationName , std::vector<int> frameList, float frameDisplayTime )
{
	ASSERT( m_spriteSheetID != -1 , LOG_FATAL , "Trying to create an animation without any Spritesheet loaded" );
	ASSERT( GC::GetActiveRenderManager()->GetAnimation( animationName ) == nullptr , LOG_FATAL , "Trying to create a new animation with an existent animation's name" );
	GCAnimation* pNewAnimation = new GCAnimation();
	pNewAnimation->SetSpriteSheet( m_spritesheetName , m_spriteSheetID);

	for ( int i = 0; i < frameList.size() ; i++ )
		pNewAnimation->AddFrame( frameList[i], frameDisplayTime );
	GC::GetActiveRenderManager()->AddAnimation( pNewAnimation , animationName );

	return pNewAnimation;
}






void GCSoundMixer::CopyTo( GCComponent* pDestination )
{ GCComponent::CopyTo( pDestination ); }






GCCamera::GCCamera()
{
	m_position.SetZero();
	m_target.SetZero();
	m_up.SetZero();
	
	m_viewWidth = GC::GetWindow()->GetClientWidth()/100;
	m_viewHeight = GC::GetWindow()->GetClientHeight()/100;
	m_nearZ = 1.0f;
	m_farZ = 1000.0f;
	
	m_viewMatrix.SetIdentity();
	m_projectionMatrix.SetIdentity();
}



void GCCamera::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCCamera* pCamera = static_cast<GCCamera*>( pDestination );
	
	pCamera->m_position = m_position;
	pCamera->m_target = m_target;
	pCamera->m_up = m_up;
	
	pCamera->m_nearZ = m_nearZ;
    pCamera->m_farZ = m_farZ;
    pCamera->m_viewWidth = m_viewWidth;
    pCamera->m_viewHeight = m_viewHeight;
	
	pCamera->m_viewMatrix = m_viewMatrix;
	pCamera->m_projectionMatrix = m_projectionMatrix;
}

void GCCamera::Update()
{
	bool dirty = false;
	
	GCVEC3 worldPosition = m_pGameObject->m_transform.GetWorldPosition();
	if ( m_position != worldPosition )
	{
		m_position = worldPosition;
		m_target = m_position;
		m_target.z = 0;
		dirty = true;
	}
	
	GCMATRIX worldRotationMatrix = m_pGameObject->m_transform.GetWorldRotationMatrix();
	GCVEC3 worldUp( worldRotationMatrix._21, worldRotationMatrix._22, worldRotationMatrix._23 );
	if ( m_up != worldUp )
	{
		m_up = worldUp;
		dirty = true;
	}
	
	if ( dirty == false )
		return;
	
	GC::GetActiveRenderManager()->m_pGraphics->CreateViewProjConstantBuffer( m_position, m_target, m_up, 0.0f, 0.0f, m_nearZ, m_farZ, m_viewWidth, m_viewHeight, GC_PROJECTION_TYPE::ORTHOGRAPHIC, m_projectionMatrix, m_viewMatrix );
}






GCText::GCText()
{
	m_pGeometry = nullptr;
	m_pMesh = nullptr;
	m_color = GCColor( 255, 255, 255 );
};

void GCText::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCText* pText = static_cast<GCText*>( pDestination );

	pText->m_pMesh = m_pMesh;
	pText->m_pGeometry = m_pGeometry;
	pText->m_text = m_text;	
}

void GCText::SetText(std::string text, GCColor color)
{
	m_color = color;
	m_text = text;
	GC::GetActiveTextManager()->CreateText( this );
}

void GCText::SetColor(GCColor color)
{
	m_color = color;
	GC::GetActiveTextManager()->CreateText( this );
}

void GCText::Render()
{
	GCGraphics* pGraphics = GC::GetActiveRenderManager()->m_pGraphics;
	pGraphics->UpdateWorldConstantBuffer( GC::GetActiveTextManager()->m_pMaterial, m_pGameObject->m_transform.GetWorldMatrix() );
	pGraphics->GetRender()->DrawObject( m_pMesh, GC::GetActiveTextManager()->m_pMaterial, true );
}






void GCButton::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
    GCButton* pButton = static_cast<GCButton*>( pDestination );
}



void GCButton::RegisterToManagers()
{
	GCComponent::RegisterToManagers();
	GC::GetActiveInputSystem()->m_pMouse->RegisterButton( this );
}

void GCButton::UnregisterFromManagers()
{
	GCComponent::UnregisterFromManagers();
	m_pButtonNode->Delete();
}



bool GCButton::IsClicked( GCVEC2* pMousePosition )
{
	GCVEC3 gameObjectPosition = m_pGameObject->m_transform.GetWorldPosition();
	GCVEC3 gameObjectSize = m_pGameObject->m_transform.GetWorldPosition();
	return gameObjectPosition.x < pMousePosition->x && pMousePosition->x < gameObjectPosition.x + gameObjectSize.x &&
		   gameObjectPosition.y < pMousePosition->y && pMousePosition->y < gameObjectPosition.y + gameObjectSize.y;
}





GCScript::GCScript()
{ m_pTriggerNode = nullptr; }



void GCScript::RegisterToManagers()
{
	GCComponent::RegisterToManagers();
	m_pGameObject->RegisterScriptToTrigger( this );
	// 
}

void GCScript::UnregisterFromManagers()
{
	GCComponent::UnregisterFromManagers();
	m_pTriggerNode->Delete();
}