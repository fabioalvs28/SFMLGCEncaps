#include "pch.h"
#include "FollowPlayer.h"

void ScriptFollowPlayer::CopyTo( Component* pDestination )
{
    Component::CopyTo( pDestination );
    ScriptFollowPlayer* pScript = static_cast<ScriptFollowPlayer*>( pDestination );
}

void ScriptFollowPlayer::Update()
{
    // m_pGameObject->m_transform.SetPosition( m_pPlayer->m_transform.m_position );
}