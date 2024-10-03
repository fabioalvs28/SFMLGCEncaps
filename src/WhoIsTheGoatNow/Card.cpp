#include "pch.h"
#include "Card.h"
#include "PlayerBehaviour.h"
#include "Weapon.h"
void GCScriptCard::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptCard* pNewComponent = static_cast< GCScriptCard* >( pDestination );
	pNewComponent->m_pPlayer = m_pPlayer;
	pNewComponent->m_upgrade = m_upgrade;
	pNewComponent->m_pGameScene = m_pGameScene;
}

void GCScriptCard::OnClick()
{
	GCScriptPlayerBehaviour* pPlayerScript = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>();
	int weaponIndex = pPlayerScript->GetWeaponIndex();
	GCGameObject* pWeaponGO = m_pPlayer->GetChildren().GetFirstNode()->GetData();

	switch ( m_upgrade )
	{
	case AttackSpeed:
		switch ( weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeAttackSpeed( 0.03f );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript1 = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript1->UpgradeAttackSpeed( 0.02f );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript2 = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript2->UpgradeAttackSpeed( 0.05f );
			break;
		}
		}
		break;
		

	case ShootingSpeed:
		switch ( weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeShootCooldown( -0.02 );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript1 = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript1->UpgradeShootCooldown( -0.05 );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript2 = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript2->UpgradeShootCooldown( -0.1 );
			break;
		}
		}
		break;
		
	case Range:
		switch ( weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeRange( 2.0f );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript1 = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript1->UpgradeRange( 1.5f );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript2 = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript2->UpgradeRange( 1 );
			break;
		}
		}
		break;
	case Penetration:
		switch ( weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradePenetration( 1 );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript1 = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript1->UpgradePenetration( 1 );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript2 = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript2->UpgradePenetration( 0.5f );
			break;
		}
		}
		break;
	case WalkSpeed:
		pPlayerScript->UpgradeVelocity( 0.2 );
		break;

	case PV:
		pPlayerScript->UpgradeHP( 2 );
		break;

	case MaxHealth:
		pPlayerScript->UpgradeMaxHealth( 1 );
		break;
		
	}

	m_pGameScene->SetActive();
}