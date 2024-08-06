#include "pch.h"
#include "FollowPlayer.h"

void GCScriptFollowPlayer::CopyTo( GCComponent* pDestination )
{
    GCComponent::CopyTo( pDestination );
    GCScriptFollowPlayer* pScript = static_cast<GCScriptFollowPlayer*>( pDestination );
}

void GCScriptFollowPlayer::Update()
{
    // m_pGameObject->m_transform.SetPosition( m_pPlayer->m_transform.m_position );
}