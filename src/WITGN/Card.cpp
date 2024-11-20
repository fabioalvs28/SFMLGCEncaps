#include "pch.h"
#include "Card.h"
#include "PlayerBehaviour.h"
#include "Weapon.h"
void GCScriptCard::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptCard* pNewComponent = static_cast< GCScriptCard* >( pDestination );
	pNewComponent->m_pPlayer = m_pPlayer;
	//pNewComponent->m_upgrade = m_upgrade;
	pNewComponent->m_pGameScene = m_pGameScene;
}

void GCScriptCard::ALED()
{
	m_weaponIndex = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->GetWeaponIndex();

	GCSprite* SP_AttackSpeed = new GCSprite("WhoIsTheGoatNow", std::string("attSpeed.dds" ) );
	GCSprite* SP_ShootingSpeed = new GCSprite("WhoIsTheGoatNow", std::string( "ShootingSpeed.dds" ) );
	GCSprite* SP_Range = new GCSprite("WhoIsTheGoatNow", std::string( "Range.dds" ) );
	GCSprite* SP_Pene = new GCSprite("WhoIsTheGoatNow", std::string( "Pene.dds" ) );
	GCSprite* SP_Wspeed = new GCSprite("WhoIsTheGoatNow", std::string( "WSpeed.dds" ) );
	GCSprite* SP_Hp = new GCSprite("WhoIsTheGoatNow", std::string( "hp.dds" ) );
	GCSprite* SP_MaxHp = new GCSprite("WhoIsTheGoatNow", std::string( "MaxHP.dds" ) );

	switch ( m_upgrade )
	{
	case AttackSpeed:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_AttackSpeed );
		break;
	case ShootingSpeed:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_ShootingSpeed );
		break;
	case Range:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_Range );
		break;
	case Penetration:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_Pene );
		break;
	case WalkSpeed:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_Wspeed );
		break;
	case PV:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_Hp );
		break;

	case MaxHealth:
		m_pGameObject->GetComponent<GCSpriteRenderer>()->SetSprite( SP_MaxHp );
		break;
	}
}

void GCScriptCard::OnClick()
{


	GCScriptPlayerBehaviour* pPlayerScript = m_pPlayer->GetComponent<GCScriptPlayerBehaviour>();
	
	GCGameObject* pWeaponGO = m_pPlayer->GetChildren().GetFirstNode()->GetData();

	switch ( m_upgrade )
	{
	case AttackSpeed:
		switch ( m_weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeAttackSpeed( 0.03f );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript->UpgradeAttackSpeed( 0.02f );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript->UpgradeAttackSpeed( 0.05f );
			break;
		}
		}
		break;


	case ShootingSpeed:
		switch ( m_weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeShootCooldown( -0.02 );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript->UpgradeShootCooldown( -0.05 );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript->UpgradeShootCooldown( -0.1 );
			break;
		}
		}
		break;

	case Range:
		switch ( m_weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradeRange( 2.0f );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript->UpgradeRange( 1.5f );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript->UpgradeRange( 1 );
			break;
		}
		}
		break;
	case Penetration:
		switch ( m_weaponIndex )
		{
		case 0:
		{
			GCScriptSniper* pWeaponScript = pWeaponGO->GetComponent<GCScriptSniper>();
			pWeaponScript->UpgradePenetration( 1 );
			break;
		}
		case 1:
		{
			GCScriptShotgun* pWeaponScript = pWeaponGO->GetComponent<GCScriptShotgun>();
			pWeaponScript->UpgradePenetration( 1 );
			break;
		}
		case 2:
		{
			GCScriptMachineGun* pWeaponScript = pWeaponGO->GetComponent<GCScriptMachineGun>();
			pWeaponScript->UpgradePenetration( 0.5f );
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