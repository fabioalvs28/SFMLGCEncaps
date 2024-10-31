#include "pch.h"
#include "ExpText.h"
#include "PlayerBehaviour.h"

void GCScriptExpText::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptExpText* pNewComponent = static_cast< GCScriptExpText* >( pDestination );
	pNewComponent->m_pPlayer = m_pPlayer;
}

void GCScriptExpText::Update()
{
	int playerExp = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetExp();
	int playerLevel = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetLevel();
	int playerLevelAmount = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetLevelAmount();
	m_pGameObject->GetComponent<GCText>()->SetText("LVL : " + std::to_string( playerLevelAmount ) + "(" + std::to_string(playerExp) + "/" + std::to_string(playerLevel) + ")");
}