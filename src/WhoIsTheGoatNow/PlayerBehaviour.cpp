#include "pch.h"
#include "PlayerBehaviour.h"
#include "Weapon.h"
#include "Card.h"

void GCScriptPlayerBehaviour::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptPlayerBehaviour* pNewComponent = static_cast<GCScriptPlayerBehaviour*>(pDestination);
}

void GCScriptPlayerBehaviour::Start()
{
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
	m_pAnimator = m_pGameObject->GetComponent<GCAnimator>();

	m_velocity = 0.05f;
	m_life = 3;
	m_hp = m_life;
	m_exp = 0;
	m_levelAmount = 0;
	m_level = 5;
}


void GCScriptPlayerBehaviour::Update()
{
	if (GCINPUTS::GetKeyDown(GCKEYBOARD::ESC))
		m_pPauseScene->SetActive();
}

void GCScriptPlayerBehaviour::FixedUpdate()
{

	GCVEC3 translation;
	//Player movements
	for (int input : m_pInputSystem->GetDirections()->Up.inputs) //Up
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation += GCVEC3::Up();
			m_pAnimator->PlayAnimation("PlayerBackward", true);
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Down.inputs) //Down
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation -= GCVEC3::Up();
			m_pAnimator->PlayAnimation("PlayerForward", true);
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Left.inputs) //Left
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation -= GCVEC3::Right();
			m_pAnimator->PlayAnimation("PlayerLeft", true);
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Right.inputs) //Right
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation += GCVEC3::Right();
			m_pAnimator->PlayAnimation("PlayerRight", true);
		}
	}

	//apply translation
	translation *= m_velocity;
	m_pGameObject->m_transform.Translate(translation);

	if ( m_exp >= m_level )
		LevelUp();
}

void GCScriptPlayerBehaviour::SetWeapon( int weapon )
{
	m_weaponIndex = weapon;
	switch ( weapon )
	{
	case Sniper:
		m_pGameObject->GetChildren().GetFirstNode()->GetData()->Activate();
		break;
	case Shotgun:
		m_pGameObject->GetChildren().GetLastNode()->GetData()->Activate();
		break;
	case MachineGun:
		m_pGameObject->GetChildren().GetFirstNode()->GetNext()->GetData()->Activate();
		break;
	default:
		m_pGameObject->GetChildren().GetFirstNode()->GetData()->Activate();
		break;
	}
}

void GCScriptPlayerBehaviour::Hit( int damage )
{
	m_hp -= damage;
	if ( m_hp <= 0 )
	{
		Die();
	}
}

void GCScriptPlayerBehaviour::Die()
{
	m_pGameObject->m_transform.SetPosition( GCVEC3( 0 , 0 , 0 ) );
	m_hp = m_life;
	m_pDeathScene->SetActive();
}

void GCScriptPlayerBehaviour::LevelUp()
{
	m_exp -= m_level;
	m_level += 2;
	m_levelAmount += 1;
	for ( int i = -1; i < 2; i++ )
	{
		GCGameObject* newCard = m_pCardTemplate->Duplicate();
		newCard->GetComponent<GCScriptCard>()->SetUpgrade(rand() % 8);
		newCard->m_transform.SetPosition(GCVEC3(i * 2, 0,0));
	}
	m_pUpgradeScene->SetActive();
}