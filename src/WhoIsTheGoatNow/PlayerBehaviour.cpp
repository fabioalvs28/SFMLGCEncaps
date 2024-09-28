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
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Down.inputs) //Down
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation -= GCVEC3::Up();
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Left.inputs) //Left
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation -= GCVEC3::Right();
		}
	}

	for (int input : m_pInputSystem->GetDirections()->Right.inputs) //Right
	{
		if (GCINPUTS::GetKeyStay(GCKEYBOARD(input)))
		{
			translation += GCVEC3::Right();
		}
	}

	//apply translation
	translation *= m_velocity;
	m_pGameObject->m_transform.Translate(translation);
}

void GCScriptPlayerBehaviour::Update()
{

}

void GCScriptPlayerBehaviour::Shoot()
{
}

