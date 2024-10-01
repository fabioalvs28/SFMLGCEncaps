#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Bullet.h"
#include "Test.h";
#include "Mouse.h"
#include "Weapon.h"
#include "ButtonSelect.h"

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );

#pragma region GAMESCENE
    GCScene* SC_pGame = GCScene::Create();
    
    GCSprite SP_enemy("red_square.dds");

    //GCGameObject* ptest = pScene->CreateGameObject();
    //ptest->AddComponent<GCText>();
    //ptest->AddComponent<GCScriptTest>()->pText = ptest->GetComponent<GCText>();
    //ptest->m_transform.SetPosition(GCVEC3(-4, 3, 0));

    //GCGameObject* GO_pGameScene = pScene->CreateGameObject();

    //Bullet prefab 

    //Player creation
    GCSprite SP_player("blue_square.dds");

    GCGameObject* GO_pPlayer = SC_pGame->CreateGameObject();
    GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_player);
    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>();
    GCAnimator* pAnimator = GO_pPlayer->AddComponent<GCAnimator>();
    pAnimator->LoadSpriteSheet("spritesheet_0.dds", 0);
    pAnimator->CreateAnimation("aaa", 0, 5, 0.3f);
    pAnimator->PlayAnimation("aaa",true);
    GO_pPlayer->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pPlayer->AddTag("player");
    GO_pPlayer->SetLayer(1);

    //Enemy prefab

    GCGameObject* GO_pEnemy = SC_pGame->CreateGameObject();
    GO_pEnemy->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    GO_pEnemy->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pEnemy->AddComponent<GCScriptEnemyBehaviour>()->SetTarget(GO_pPlayer);
    GO_pEnemy->AddTag("enemy");
    GO_pEnemy->Deactivate();

    GCGameObject* GO_pEnemySpawner = SC_pGame->CreateGameObject();
    GO_pEnemySpawner->AddComponent<GCScriptEnemySpawner>()->SetTemplate(GO_pEnemy);

    //SniperPrefab
    GCSprite SP_sniper("Shotgun.dds");
    GCSprite SP_sniperBullet("bullet.dds");
    GCGameObject* GO_pSniper = GO_pPlayer->CreateChild();
    GO_pSniper->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_sniper );
    GO_pSniper->AddComponent<GCScriptSniper>()->SetBulletSprite(&SP_sniperBullet);

    //MachineGune prefab
    GCSprite SP_machineGun("Shotgun.dds");
    GCSprite SP_machineGunBullet("bulletMG.dds");
    GCGameObject* GO_pMachineGun = GO_pPlayer->CreateChild();
    GO_pMachineGun->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_machineGun );
    GO_pMachineGun->AddComponent<GCScriptMachineGun>()->SetBulletSprite( &SP_machineGunBullet );

    //Shotgun prefab
    GCSprite SP_shotgun("Shotgun.dds");
    GCSprite SP_shotgunBullet("bulletSG.dds");
    GCGameObject* GO_pShotgun = GO_pPlayer->CreateChild();
    GO_pShotgun->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_shotgun );
    GO_pShotgun->AddComponent<GCScriptShotgun>()->SetBulletSprite( &SP_shotgunBullet );

    //bullet prefab
    GCGameObject* GO_pBullet = SC_pGame->CreateGameObject();
    GO_pBullet->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pBullet->AddComponent<GCScriptBullet>();
    GO_pBullet->AddTag("bullet");
    GO_pBullet->Deactivate();

    GO_pSniper->GetComponent<GCScriptSniper>()->SetBulletTemplate( GO_pBullet );
    GO_pMachineGun->GetComponent<GCScriptMachineGun>()->SetBulletTemplate( GO_pBullet );
    GO_pShotgun->GetComponent<GCScriptShotgun>()->SetBulletTemplate( GO_pBullet );

    GO_pSniper->Deactivate();
    GO_pMachineGun->Deactivate();
    GO_pShotgun->Deactivate();

    //GO_pPlayer->GetComponent<GCScriptPlayerBehaviour>()->SetWeapon( 0 );
    
#pragma endregion

#pragma region HOMESCENE

    GCScene* SC_pHome = GCScene::Create();
    //pHomeScene->SetActive();

    GCGameObject* GO_Title = SC_pHome->CreateGameObject();
    GO_Title->m_transform.SetPosition(GCVEC3(0, 0, 0));
    GO_Title->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_Title->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    GO_Title->AddComponent<GCText>()->SetText("WHO \tIS\t THE \bGOAT\b NOW", GCColor(0,0,255));
    GO_Title->AddComponent<GCScriptTest>()->pText = GO_Title->GetComponent<GCText>();
    GO_Title->AddComponent<GCButton>();

    //Button prefab
    //GCGameObject* GO_pButton = pHomeScene->CreateGameObject();
    //GO_pButton->AddComponent<GCSpriteRenderer>();
    //GO_pButton->AddComponent<GCBoxCollider>();
    
    //Mouse collider
    //GCGameObject* GO_pMouse = pHomeScne->CreateGameObject();
    //GO_pMouse->AddComponent<GCBoxCollider>()->SetVisible(true);
    //GO_pMouse->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    //GO_pMouse->AddComponent<GCScriptMouse>();
    //GO_pMouse->m_transform.Scale(0.25f);
    //GO_pMouse->SetLayer(1);


#pragma endregion

#pragma region SELECTWEAPONSCENE
    
    GCScene* SC_pWeaponSelect = GCScene::Create();
    SC_pWeaponSelect->SetActive();

    GCGameObject* GO_pButtonSelect = SC_pWeaponSelect->CreateGameObject();
    GO_pButtonSelect->AddComponent<GCButton>();
    GCScriptButtonSelect* SR_pButtonSelect = GO_pButtonSelect->AddComponent<GCScriptButtonSelect>();
    SR_pButtonSelect->SetGameScene( SC_pGame );
    SR_pButtonSelect->SetPlayer( GO_pPlayer );
    GO_pButtonSelect->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_enemy );
    
    GCGameObject* GO_pSelectSniper = GO_pButtonSelect->Duplicate();
    GO_pSelectSniper->GetComponent<GCScriptButtonSelect>()->SetWeapon( 0 );
    GO_pSelectSniper->m_transform.Translate( GCVEC3( -2.0f , 0.0f , 0.0f ) );

    GCGameObject* GO_pSelectSG = GO_pButtonSelect->Duplicate();
    GO_pSelectSG->GetComponent<GCScriptButtonSelect>()->SetWeapon( 1 );

    GCGameObject* GO_pSelectMG = GO_pButtonSelect->Duplicate();
    GO_pSelectMG->GetComponent<GCScriptButtonSelect>()->SetWeapon( 2 );
    GO_pSelectMG->m_transform.Translate( GCVEC3( 2.0f , 0.0f , 0.0f ) );

#pragma endregion
    pGameManager->Run();
    return 0;
}