#include "pch.h"
#include "ButtonSelect.h"
#include "PlayerBehaviour.h"

void GCScriptButtonSelect::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptButtonSelect* pNewComponent = static_cast< GCScriptButtonSelect* >( pDestination );
	pNewComponent->m_pWeaponIndex = m_pWeaponIndex;
	pNewComponent->m_pGameScene = m_pGameScene;
	pNewComponent->m_pPlayer = m_pPlayer;
}

void GCScriptButtonSelect::OnClick()
{
	m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->SetWeapon( m_pWeaponIndex );
	m_pGameScene->SetActive();
}