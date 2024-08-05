#include "pch.h"
#include "Movable.h"

void ScriptMovable::Update()
{
    if ( m_pKeyboard->GetKeyStay( m_pKeyboard->Z ) )
        m_pGameObject->m_transform.Translate( GCVEC3( 0.0f, 0.1f, 0.0f ) );
    if ( m_pKeyboard->GetKeyStay( m_pKeyboard->Q ) )
        m_pGameObject->m_transform.Translate( GCVEC3( -0.1f, 0.0f, 0.0f ) );
    if ( m_pKeyboard->GetKeyStay( m_pKeyboard->S ) )
        m_pGameObject->m_transform.Translate( GCVEC3( 0.0f, -0.1f, 0.0f ) );
    if ( m_pKeyboard->GetKeyStay( m_pKeyboard->D ) )
        m_pGameObject->m_transform.Translate( GCVEC3( 0.1f, 0.0f, 0.0f ) );
}



void ScriptMovable::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
}