#include "GCEngine.h"
enum PossibleUpgrades
{
	AttackSpeed,
	ShootingSpeed,
	WalkSpeed,
	Range,
	Penetration,
	PV,
	MaxHealth,
};

CREATE_SCRIPT_START(Card)
public:
	void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; };
	void OnClick() override;
	void SetUpgrade( int upgrade ) { m_upgrade = upgrade; };

private:
	GCGameObject* m_pPlayer;
	int m_upgrade;

CREATE_SCRIPT_END