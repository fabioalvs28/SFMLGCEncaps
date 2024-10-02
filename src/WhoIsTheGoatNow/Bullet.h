#pragma once

CREATE_SCRIPT_START(Bullet)
public:
	void Start() override;
	void FixedUpdate() override;
	void Shoot(int bulletIndex = 0);
	void OnTriggerStay(GCCollider* pCollider) override;

	void SetDirection( GCVEC3 newDirection ) { m_direction = newDirection; }
	GCVEC3 GetDirection() { return m_direction; }
	
	void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; }
	void SetOrigin( GCGameObject* pOrigin ) { m_pOrigin = pOrigin; }

	void SetAttackSpeed( float attackSpeed ) { m_attackSpeed = attackSpeed; }
	void SetRange( float range ) { m_range = range; }
	void SetPenetration( int penetration ) { m_penetration = penetration; }

private:
	float m_attackSpeed;
	float m_range;
	int m_penetration;

	bool m_destroyed;
	bool m_isFlipped;
	float m_startRotationZ;

	GCVEC3 m_centralizer;
	GCVEC3 m_startPosition;
	GCVEC3 m_direction;
	GCCamera* m_pCamera;
	GCGameObject* m_pPlayer;
	GCGameObject* m_pOrigin;

CREATE_SCRIPT_END