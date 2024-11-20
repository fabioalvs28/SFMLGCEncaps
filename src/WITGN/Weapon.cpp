#include "pch.h"
#include "Weapon.h"
#include "Bullet.h"

# define M_PIl          3.141592653589793238462643383279502884L 

void GCScriptWeapon::CopyTo(GCComponent* pDestination)
{
	GCComponent::CopyTo(pDestination);
	GCScriptWeapon* pNewComponent = static_cast<GCScriptWeapon*>(pDestination);
	pNewComponent->m_penetration = m_penetration;
	pNewComponent->m_range = m_range;
	pNewComponent->m_attackSpeed = m_attackSpeed;
	pNewComponent->m_shootCooldown = m_shootCooldown;
	pNewComponent->m_pBulletTemplate = m_pBulletTemplate;
	pNewComponent->m_pInputSystem = m_pInputSystem;
	pNewComponent->m_currentDeltaTime = m_currentDeltaTime;
	pNewComponent->m_pBulletSprite = m_pBulletSprite;
}

void GCScriptWeapon::Start()
{
	m_currentDeltaTime;
	m_isFlipped = false;
	m_centralizer = m_pGameObject->GetParent()->m_transform.GetWorldScale() / 2;
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();

	//weapon attributes & characteristics
	m_attackSpeed = 0.05f;
	m_range = 5.5f;
	m_penetration = 1;

	m_shootCooldown = 1.25f;

}

void GCScriptWeapon::Update()
{
	m_currentDeltaTime += 0.01f;
	GCVEC3 position = m_pGameObject->GetParent()->m_transform.m_position;

	float rotationSpeed = 0.019f;

	float deltaX = GCINPUTS::GetMousePos().x - position.x;
	float deltaY = GCINPUTS::GetMousePos().y - position.y;

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

	m_pGameObject->GetComponent<GCSpriteRenderer>()->SetOrigin( Left );
    m_pGameObject->m_transform.SetPosition(m_centralizer);

	m_pGameObject->m_transform.SetRotation(0.0f,0.0f,angleDegrees * rotationSpeed);

	//Actions
	for ( int input : m_pInputSystem->GetActions()->Shoot.inputs ) //Shoot
	{
		if ( GCINPUTS::GetKeyDown( GCKEYBOARD( input ) ) && m_currentDeltaTime >= m_shootCooldown )
		{
			m_currentDeltaTime = 0;
			Shoot();
		}
	}
}

void GCScriptWeapon::Shoot()
{
	GCGameObject* newBullet = m_pBulletTemplate->Duplicate();
	newBullet->Activate();

	newBullet->AddComponent<GCSpriteRenderer>()->SetSprite( m_pBulletSprite );
	newBullet->GetComponent<GCScriptBullet>()->SetAttackSpeed( m_attackSpeed );
	newBullet->GetComponent<GCScriptBullet>()->SetRange( m_range );
	newBullet->GetComponent<GCScriptBullet>()->SetPenetration( m_penetration );

	newBullet->GetComponent<GCScriptBullet>()->SetOrigin( m_pGameObject );
	newBullet->GetComponent<GCScriptBullet>()->SetPlayer( m_pGameObject->GetParent() );
	newBullet->GetComponent<GCScriptBullet>()->Shoot();
}

//=============================SNIPER=============================
void GCScriptSniper::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptSniper* pNewComponent = static_cast< GCScriptSniper* >( pDestination );
	pNewComponent->m_penetration = m_penetration;
	pNewComponent->m_range = m_range;
	pNewComponent->m_attackSpeed = m_attackSpeed;
	pNewComponent->m_shootCooldown = m_shootCooldown;
	pNewComponent->m_pBulletTemplate = m_pBulletTemplate;
	pNewComponent->m_pInputSystem = m_pInputSystem;
	pNewComponent->m_currentDeltaTime = m_currentDeltaTime;
	pNewComponent->m_pBulletSprite = m_pBulletSprite;
}


void GCScriptSniper::Start()
{
	//weapon attributes & characteristics
	m_attackSpeed = 0.3f;
	m_range = 7.5f;
	m_penetration = 3;

	m_shootCooldown = 1.25f;

	m_centralizer = m_pGameObject->GetParent()->m_transform.GetWorldScale() / 2;

	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
}

//=============================MACHINEGUN=============================
void GCScriptMachineGun::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptMachineGun* pNewComponent = static_cast< GCScriptMachineGun* >( pDestination );
	pNewComponent->m_penetration = m_penetration;
	pNewComponent->m_range = m_range;
	pNewComponent->m_attackSpeed = m_attackSpeed;
	pNewComponent->m_shootCooldown = m_shootCooldown;
	pNewComponent->m_pBulletTemplate = m_pBulletTemplate;
	pNewComponent->m_pInputSystem = m_pInputSystem;
	pNewComponent->m_currentDeltaTime = m_currentDeltaTime;
	pNewComponent->m_pBulletSprite = m_pBulletSprite;
}

void GCScriptMachineGun::Start()
{
	//weapon attributes & characteristics
	m_attackSpeed = 0.05f;
	m_range = 3.5f;
	m_penetration = 0;

	m_shootCooldown = 0.05f;

	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
	m_centralizer = m_pGameObject->GetParent()->m_transform.GetWorldScale() / 2;

}

void GCScriptMachineGun::Update()
{
	m_currentDeltaTime += 0.01f;
	GCVEC3 position = m_pGameObject->GetParent()->m_transform.m_position;

	float rotationSpeed = 0.019f;

	float deltaX = GCINPUTS::GetMousePos().x - position.x;
	float deltaY = GCINPUTS::GetMousePos().y - position.y;

	float angle = atan2f( deltaX , deltaY );

	float angleDegrees = angle * ( 180.0f / M_PIl ) - 90;

	if ( GCINPUTS::GetMousePos().x < 0 && m_isFlipped == false )
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = true;
	}
	else if ( GCINPUTS::GetMousePos().x > 0 && m_isFlipped )
	{
		m_pGameObject->GetComponent<GCSpriteRenderer>()->FlipY();
		m_isFlipped = false;
	}

	m_pGameObject->GetComponent<GCSpriteRenderer>()->SetOrigin( Left );
	m_pGameObject->m_transform.SetPosition( m_centralizer );

	m_pGameObject->m_transform.SetRotation( 0.0f , 0.0f , angleDegrees * rotationSpeed );

	//Actions
	for ( int input : m_pInputSystem->GetActions()->Shoot.inputs ) //Shoot
	{
		if ( GCINPUTS::GetKeyStay( GCKEYBOARD( input ) ) && m_currentDeltaTime >= m_shootCooldown )
		{
			m_currentDeltaTime = 0;
			Shoot();
		}
	}
}


//=============================SHOTGUN=============================
void GCScriptShotgun::CopyTo( GCComponent* pDestination )
{
	GCComponent::CopyTo( pDestination );
	GCScriptShotgun* pNewComponent = static_cast< GCScriptShotgun* >( pDestination );
	pNewComponent->m_penetration = m_penetration;
	pNewComponent->m_range = m_range;
	pNewComponent->m_attackSpeed = m_attackSpeed;
	pNewComponent->m_shootCooldown = m_shootCooldown;
	pNewComponent->m_pBulletTemplate = m_pBulletTemplate;
	pNewComponent->m_pInputSystem = m_pInputSystem;
	pNewComponent->m_currentDeltaTime = m_currentDeltaTime;
	pNewComponent->m_bulletAmount = m_bulletAmount;
	pNewComponent->m_pBulletSprite = m_pBulletSprite;
}

void GCScriptShotgun::Start()
{
	//weapon attributes & characteristics
	m_attackSpeed = 0.3f;
	m_range = 5.0f;
	m_penetration = 1;

	m_shootCooldown = 1.0f;
	m_pInputSystem = new InputSystem();
	m_pInputSystem->Initialize();
	m_centralizer = m_pGameObject->GetParent()->m_transform.GetWorldScale() / 2;

	m_bulletAmount = 3;

}

void GCScriptShotgun::Shoot()
{
	for ( int i = -1; i < m_bulletAmount / 2 + 1; i++ )
	{
		GCGameObject* newBullet = m_pBulletTemplate->Duplicate();
		newBullet->Activate();

		newBullet->AddComponent<GCSpriteRenderer>()->SetSprite( m_pBulletSprite );
		newBullet->GetComponent<GCScriptBullet>()->SetAttackSpeed( m_attackSpeed );
		newBullet->GetComponent<GCScriptBullet>()->SetRange( m_range );
		newBullet->GetComponent<GCScriptBullet>()->SetPenetration( m_penetration );

		newBullet->GetComponent<GCScriptBullet>()->SetOrigin( m_pGameObject );
		newBullet->GetComponent<GCScriptBullet>()->SetPlayer( m_pGameObject->GetParent() );
		newBullet->GetComponent<GCScriptBullet>()->Shoot();
		newBullet->GetComponent<GCScriptBullet>()->Shoot( i );
	}
}