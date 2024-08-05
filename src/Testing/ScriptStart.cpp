#include "pch.h"
#include "ScriptStart.h"

void ScriptStart::Start()
{
    pKeyboard = GC::GetActiveInputSystem()->m_pKeyboard;
    pMouse = GC::GetActiveInputSystem()->m_pMouse;
}

void ScriptStart::Update()
{

    Animator* pAnimator = m_pGameObject->GetComponent<Animator>();
    if (pKeyboard->GetKeyStay( pKeyboard->Q ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( -0.1f , 0.0f , 0.0f ) );
        pAnimator->PlayAnimation( "WalkHeroL", false );
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->D ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3( 0.1f, 0.0f, 0.0f) );
        pAnimator->PlayAnimation( "WalkHeroR", true);
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->Z ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3(0.0f,0.1f , 0.0f) );
        pAnimator->PlayAnimation( "WalkBackward", true);
    }
    if ( pKeyboard->GetKeyStay( pKeyboard->S ) )
    {
        m_pGameObject->m_transform.Translate( GCVEC3(0.0f, -0.1f, 0.0f) );
        pAnimator->PlayAnimation( "WalkForward", true);
    }
}

void ScriptStart::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptStart* pScript = static_cast<ScriptStart*>( pDestination );
    pScript->pKeyboard = pKeyboard;
    pScript->pMouse = pMouse;
}