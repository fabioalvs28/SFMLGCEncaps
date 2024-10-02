#include "pch.h"
#include "PlayerBehaviour.h"
#include "Weapon.h"


void GCScriptPlayerBehaviour::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptPlayerBehaviour* pNewComponent = static_cast<GCScriptPlayerBehaviour*>(pDestination);
}

void GCScriptPlayerBehaviour::Start()
{
	m_velocity = 0.05f;
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
	m_pAnimator = m_pGameObject->GetComponent<GCAnimator>();
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
}

void GCScriptPlayerBehaviour::SetWeapon( int weapon )
{
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

