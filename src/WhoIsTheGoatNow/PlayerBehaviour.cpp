#include "pch.h"
#include "PlayerBehaviour.h"
#include "Bullet.h"

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

void GCScriptPlayerBehaviour::Update()
{
	//Actions
	for (int input : m_pInputSystem->GetActions()->Shoot.inputs) //Shoot
	{
		if (GCINPUTS::GetKeyDown(GCKEYBOARD(input)))
		{
			PlayerShoot();
		}
	}
}

void GCScriptPlayerBehaviour::PlayerShoot()
{
	GCGameObject* newBullet = m_pBulletTemplate->Duplicate();
	newBullet->Activate();
	newBullet->GetComponent<GCScriptBullet>()->BulletShoot();
}

