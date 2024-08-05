#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start()
{
    //pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    //pMouse = GC::GetActiveInputSystem()->m_pMouse;
}

void ScriptStart::Update()
{
    GCKeyboardInputManager* pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    GCMouseInputManager* pMouse = GC::GetActiveInputSystem()->m_pMouse;
    Animator* pAnimator = m_pGameObject->GetComponent<Animator>();
    if ( KEYBOARD::GetKeyStay( KEYBOARD::Q ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( -0.01 , 0 , 0 ) );
        pAnimator->PlayAnimation( "WalkHeroL", true );
    }
    if ( KEYBOARD::GetKeyStay( KEYBOARD::D ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0.01, 0 , 0 ) );
        pAnimator->PlayAnimation( "WalkHeroR", true);
    }
    if ( KEYBOARD::GetKeyStay( KEYBOARD::Z ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0,0.01 , 0 ) );
        pAnimator->PlayAnimation( "WalkBackward", true);
    }
    if ( KEYBOARD::GetKeyStay( KEYBOARD::S ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0, -0.01, 0 ) );
        pAnimator->PlayAnimation( "WalkForward", true);
    }
}

void ScriptStart::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptStart* pScript = static_cast<ScriptStart*>( pDestination );
    //pScript->pKeyboard = pKeyboard;
    //pScript->pMouse = pMouse;
}