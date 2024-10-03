#include "GCEngine.h"
enum PossibleUpgrades
{
	AttackSpeed = 3,
	ShootingSpeed = 4,
	Range = 5,
	Penetration = 6,
	WalkSpeed = 7,
	PV = 8,
	MaxHealth = 9,
};

CREATE_SCRIPT_START(Card)
public:
	void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; };
	void OnClick() override;
	void ALED();
	void SetUpgrade( int upgrade ) { m_upgrade = upgrade; };
	void SetGameScene( GCScene* pGameScene ) { m_pGameScene = pGameScene; }

private:
	GCGameObject* m_pPlayer;
	int m_upgrade;
	GCScene* m_pGameScene;
	int m_weaponIndex;


CREATE_SCRIPT_END