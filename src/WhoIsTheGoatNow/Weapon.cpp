#include "pch.h"
#include "Weapon.h"
#include "Bullet.h"

# define M_PIl          3.141592653589793238462643383279502884L 

void GCScriptWeapon::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptWeapon* pNewComponent = static_cast<GCScriptWeapon*>(pDestination);
	pNewComponent->m_direction = m_direction;
	pNewComponent->m_distanceToPlayer = m_distanceToPlayer;
	pNewComponent->m_penetration = m_penetration;
	pNewComponent->m_range = m_range;
	pNewComponent->m_speed = m_speed;
	pNewComponent->m_shootCooldown = m_shootCooldown;
	pNewComponent->m_pBulletTemplate = m_pBulletTemplate;
}

void GCScriptWeapon::Start()
{
	m_distanceToPlayer = 0.0f;
	m_isFlipped = false;
	m_centralizer = m_pGameObject->GetParent()->m_transform.GetWorldScale() / 2;
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
}

void GCScriptWeapon::Update()
{
	GCVEC3 position = m_pGameObject->m_transform.m_position;

	float rotationSpeed = 0.019f;

	float deltaX = GCINPUTS::GetMousePos().x - position.x + m_centralizer.x;
	float deltaY = GCINPUTS::GetMousePos().y - position.y + m_centralizer.y;

	float angle = atan2f(deltaX, deltaY);

	float angleDegrees = angle * (180.0f / M_PIl) - 90;

	if ( GCINPUTS::GetMousePos().x < 0 && m_isFlipped == false)
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = true;
	}
	else if (GCINPUTS::GetMousePos().x > 0 && m_isFlipped)
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = false;
	}

	m_distanceToPlayer = 0.01f;
	m_pGameObject->GetComponent<GCSpriteRenderer>()->SetOrigin( Left );
    m_pGameObject->m_transform.SetPosition(m_centralizer);

	m_pGameObject->m_transform.SetRotation(0.0f,0.0f,angleDegrees * rotationSpeed);


	//Actions
	for ( int input : m_pInputSystem->GetActions()->Shoot.inputs ) //Shoot
	{
		if ( GCINPUTS::GetKeyDown( GCKEYBOARD( input ) ) )
		{
			Shoot();
		}
	}
}

void GCScriptWeapon::Shoot()
{
	GCGameObject* newBullet =  m_pBulletTemplate->Duplicate();
	newBullet->Activate();
	
	//m_pGameObject->AddChild( newBullet );
	newBullet->GetComponent<GCScriptBullet>()->SetOrigin(m_pGameObject);
	newBullet->GetComponent<GCScriptBullet>()->SetPlayer(m_pGameObject->GetParent());
	newBullet->GetComponent<GCScriptBullet>()->Shoot();
}