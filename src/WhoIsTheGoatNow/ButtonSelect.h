#pragma once
#include "GCEngine.h"

CREATE_SCRIPT_START(ButtonSelect)
public:
	void OnClick() override;
	void SetWeapon( int weaponIndex ) { m_pWeaponIndex = weaponIndex; }
	void SetGameScene( GCScene* gameScene ) { m_pGameScene = gameScene; }
	void SetPlayer( GCGameObject* pPlayer ) { m_pPlayer = pPlayer; }

private:
	int m_pWeaponIndex;
	GCScene* m_pGameScene;
	GCGameObject* m_pPlayer;

CREATE_SCRIPT_END