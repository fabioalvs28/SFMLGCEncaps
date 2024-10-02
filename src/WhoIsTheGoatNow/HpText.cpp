#include "pch.h"
#include "HpText.h"
#include "PlayerBehaviour.h"

void GCScriptHpText::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptHpText* pNewComponent = static_cast< GCScriptHpText* >( pDestination );
	pNewComponent->m_pPlayer = m_pPlayer;
}

void GCScriptHpText::Update()
{
	int playerHealth = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetHp();
	int playerLife = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetLife();
	std::string text = "HP : " + std::to_string(playerHealth) + "/" + std::to_string(playerLife);
	m_pGameObject->GetComponent<GCText>()->SetText(text);
}