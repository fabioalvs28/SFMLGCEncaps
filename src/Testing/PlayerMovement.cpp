#include "pch.h"
#include "PlayerMovement.h"

void GCScriptPlayerMovement::CopyTo( GCComponent* pDestination )
{
    GCComponent::CopyTo( pDestination );
    GCScriptPlayerMovement* pScript = static_cast<GCScriptPlayerMovement*>( pDestination );
    pScript->m_speed = m_speed;
}

void GCScriptPlayerMovement::Start()
{
    m_speed = 0.05f;
    m_pSprite1 = new GCSprite("Testing", "goofyImage.dds" );
    m_pSprite2 = new GCSprite("Testing", "happyImage.dds" );
}

void GCScriptPlayerMovement::Update()
{
    GCVEC3 translation;
    
    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::UP_ARROW ) )
        translation += GCVEC3::Up();
    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::LEFT_ARROW ) )
        translation -= GCVEC3::Right();
    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::DOWN_ARROW ) )
        translation -= GCVEC3::Up();
    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::RIGHT_ARROW ) )
        translation += GCVEC3::Right();
    
    if ( GCINPUTS::GetKeyDown( GCKEYBOARD::SPACE ) )
    {
        GCSprite* pTemp = m_pSprite1;
        m_pSprite1 = m_pSprite2;
        m_pSprite2 = pTemp;
        m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( m_pSprite1 );
    }
    
    if ( GCINPUTS::GetKeyDown( GCKEYBOARD::CAPS_LOCK ) )
    {
        GCBoxCollider* pCollider = m_pGameObject->GetComponent<GCBoxCollider>();
        pCollider->SetVisible( pCollider->IsVisible() == false );
    }
    
    translation *= m_speed;
    m_pGameObject->m_transform.Translate( translation );
}