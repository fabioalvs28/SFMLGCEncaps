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
    
    if ( KEYBOARD::GetKeyStay( KEYBOARD::UP ) )
    { translation.SetUp(); }

    if ( KEYBOARD::GetKeyStay( KEYBOARD::LEFT ) )
    {
        translation.SetRight();
        translation *= -1;
    }

    if ( KEYBOARD::GetKeyStay( KEYBOARD::DOWN ) )
    {
        translation.SetUp();
        translation *= -1;
    }

    if ( KEYBOARD::GetKeyStay( KEYBOARD::RIGHT ) )
    { translation.SetRight(); }
    
    translation *= m_speed;
    m_pGameObject->m_transform.Translate( translation );
}