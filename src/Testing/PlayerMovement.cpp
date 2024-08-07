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
}

void GCScriptPlayerMovement::Update()
{
    GCVEC3 translation;
    
    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::UP_ARROW ) )
    { translation.SetUp(); }

    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::LEFT_ARROW ) )
    {
        translation.SetRight();
        translation *= -1;
    }

    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::DOWN_ARROW ) )
    {
        translation.SetUp();
        translation *= -1;
    }

    if ( GCINPUTS::GetKeyStay( GCKEYBOARD::RIGHT_ARROW ) )
    { translation.SetRight(); }
    
    translation *= m_speed;
    m_pGameObject->m_transform.Translate( translation );
}