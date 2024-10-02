#include "pch.h"
#include "Card.h"
#include "PlayerBehaviour.h"

void GCScriptCard::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptCard* pNewComponent = static_cast< GCScriptCard* >( pDestination );
	pNewComponent->m_pPlayer = m_pPlayer;
	pNewComponent->m_upgrade = m_upgrade;
}

void GCScriptCard::OnClick()
{
	GCScriptPlayerBehaviour* pPlayerScript = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>();
	GCGameObject* pWeapon = m_pPlayer->GetChildren().GetFirstNode()->GetData()->GetComponent<GCScript>();
	switch ( m_upgrade )
	{
	case AttackSpeed:
		
	}
}