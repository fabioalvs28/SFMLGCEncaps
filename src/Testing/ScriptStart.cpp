#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start()
{
    pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    pMouse = GC::GetActiveInputSystem()->m_pMouse;
}

void ScriptStart::Update()
{
    GCKeyboardInputManager* pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    GCMouseInputManager* pMouse = GC::GetActiveInputSystem()->m_pMouse;
    Animator* pAnimator = m_pGameObject->GetComponent<Animator>();
    if ( pKeyboard->GetKeyStay( pKeyboard->Q ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( -0.01 , 0 , 0 ) );
        pAnimator->PlayAnimation( "WalkHeroL" );
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->D ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0.01, 0 , 0 ) );
        pAnimator->PlayAnimation( "WalkHeroR" );
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->Z ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0,0.01 , 0 ) );
        pAnimator->PlayAnimation( "WalkBackward" );
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->S ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0, -0.01, 0 ) );
        pAnimator->PlayAnimation( "WalkForward" );
    }
}

void ScriptStart::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptStart* pScript = static_cast<ScriptStart*>( pDestination );
    pScript->pKeyboard = pKeyboard;
    pScript->pMouse = pMouse;
}