#include "pch.h"
#include "PlayerMovement.h"

void ScriptPlayerMovement::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptPlayerMovement* pScript = static_cast<ScriptPlayerMovement*>( pDestination );
    pScript->m_speed = m_speed;
}

void ScriptPlayerMovement::Start()
{
    m_speed = 0.05f;
}

void ScriptPlayerMovement::Update()
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