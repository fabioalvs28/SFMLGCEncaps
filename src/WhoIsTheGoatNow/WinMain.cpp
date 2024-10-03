#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Goats.h"
#include "Bullet.h"
#include "Test.h";
#include "Mouse.h"
#include "Weapon.h"
#include "Button.h"
#include "ButtonSelect.h"
#include "ExpText.h"
#include "HpText.h"
#include "Card.h"

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );

#pragma region GAMESCENE

    //Pause Scene
    GCScene* SC_pPause = GCScene::Create();


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
    GCAnimator* pAnimator = GO_pPlayer->AddComponent<GCAnimator>();
    pAnimator->LoadSpriteSheet("spritesheet_2.dds", 2);
    pAnimator->CreateAnimation("PlayerForward", 6, 2, 0.3f);
    pAnimator->CreateAnimation("PlayerBackward", 0, 2, 0.3f);
    pAnimator->CreateAnimation("PlayerLeft", 2, 2, 0.3f);
    pAnimator->CreateAnimation("PlayerRight", 4, 2, 0.3f);
    pAnimator->PlayAnimation("PlayerBackward", true);
    //GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_player);
    GO_pPlayer->AddComponent<GCBoxCollider>()->SetVisible(false);

    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>()->SetPauseScene(SC_pPause);

    GCScriptPlayerBehaviour* SR_pPlayerScript = GO_pPlayer->GetComponent<GCScriptPlayerBehaviour>();

    GO_pPlayer->AddTag("player");
    GO_pPlayer->SetLayer(1);

    //Enemy prefab


    GCGameObject* GO_pDumbGoat = SC_pGame->CreateGameObject();
    GO_pDumbGoat->AddComponent<GCSpriteRenderer>();
    GCAnimator* pDumbGoatAnimator = GO_pDumbGoat->AddComponent<GCAnimator>();
    pDumbGoatAnimator->LoadSpriteSheet("spritesheet_1.dds", 1);
    pDumbGoatAnimator->CreateAnimation("DumbGoatForward", 5, 5, 0.05f);
    pDumbGoatAnimator->CreateAnimation("DumbGoatBackWard", 0, 5, 0.05f);
    pDumbGoatAnimator->CreateAnimation("DumbGoatLeft", 22, 5, 0.05f);
    pDumbGoatAnimator->CreateAnimation("DumbGoatRight", 17, 5, 0.05f);
    pDumbGoatAnimator->CreateAnimation("DumbGoatSpawn", 10, 7, 0.2f);
    GO_pDumbGoat->AddComponent<GCBoxCollider>()->SetVisible(false);
    GO_pDumbGoat->AddComponent<GCScriptDumbGoat>()->SetTarget(GO_pPlayer);
    GO_pDumbGoat->m_transform.SetPosition(GCVEC3(5, -3, 0));
    GO_pDumbGoat->AddTag("enemy");
    GO_pDumbGoat->Deactivate();

    GCGameObject* GO_pDarkGoat = SC_pGame->CreateGameObject();
    GO_pDarkGoat->AddComponent<GCSpriteRenderer>();
    GCAnimator* pDarkGoatAnimator = GO_pDarkGoat->AddComponent<GCAnimator>();
    pDarkGoatAnimator->LoadSpriteSheet("spritesheet_0.dds", 0);
    pDarkGoatAnimator->CreateAnimation("DarkGoatForward", 5, 5, 0.05f);
    pDarkGoatAnimator->CreateAnimation("DarkGoatBackWard", 0, 5, 0.05f);
    pDarkGoatAnimator->CreateAnimation("DarkGoatLeft", 24, 5, 0.05f);
    pDarkGoatAnimator->CreateAnimation("DarkGoatRight", 19, 5, 0.05f);
    pDarkGoatAnimator->CreateAnimation("DarkGoatSummon", 10, 9, 0.2f);
    GO_pDarkGoat->AddComponent<GCBoxCollider>()->SetVisible(false);
    GO_pDarkGoat->AddComponent<GCScriptDarkGoat>()->SetTarget(GO_pPlayer);
    GO_pDarkGoat->GetComponent<GCScriptDarkGoat>()->SetSummonedEnemy(GO_pDumbGoat);
    GO_pDarkGoat->m_transform.SetPosition(GCVEC3(5, -3, 0));
    GO_pDarkGoat->AddTag("enemy");
    GO_pDarkGoat->SetLayer(1);
    GO_pDarkGoat->Deactivate();


    GCGameObject* GO_pEnemySpawner = SC_pGame->CreateGameObject();
    GO_pEnemySpawner->AddComponent<GCText>();// ->SetText( std::to_string( 0 ) );
    GO_pEnemySpawner->m_transform.SetPosition(GCVEC3(-4, 3, 0));
    GO_pEnemySpawner->AddComponent<GCScriptEnemySpawner>()->AddEnemyInList(0, GO_pDarkGoat);
    GO_pEnemySpawner->GetComponent<GCScriptEnemySpawner>()->AddEnemyInList(1, GO_pDumbGoat);

    //SniperPrefab
    GCSprite SP_sniper("sniper.dds");
    GCSprite SP_sniperBullet("bullet.dds");
    GCGameObject* GO_pSniper = GO_pPlayer->CreateChild();
    GO_pSniper->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_sniper );
    GO_pSniper->AddComponent<GCScriptSniper>()->SetBulletSprite(&SP_sniperBullet);
    GO_pSniper->SetLayer(3);

    //MachineGune prefab
    GCSprite SP_machineGun("Minigun.dds");
    GCSprite SP_machineGunBullet("bulletMG.dds");
    GCGameObject* GO_pMachineGun = GO_pPlayer->CreateChild();
    GO_pMachineGun->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_machineGun );
    GO_pMachineGun->AddComponent<GCScriptMachineGun>()->SetBulletSprite( &SP_machineGunBullet );
    GO_pMachineGun->SetLayer(3);

    //Shotgun prefab
    GCSprite SP_shotgun("Shotgun.dds");
    GCSprite SP_shotgunBullet("bulletSG.dds");
    GCGameObject* GO_pShotgun = GO_pPlayer->CreateChild();
    GO_pShotgun->AddComponent<GCSpriteRenderer>()->SetSprite( &SP_shotgun );
    GO_pShotgun->AddComponent<GCScriptShotgun>()->SetBulletSprite( &SP_shotgunBullet );
    GO_pShotgun->SetLayer(3);

    //bullet prefab
    GCGameObject* GO_pBullet = SC_pGame->CreateGameObject();
    GO_pBullet->AddComponent<GCBoxCollider>()->SetVisible(false);
    GO_pBullet->AddComponent<GCScriptBullet>();
    GO_pBullet->AddTag("bullet");
    GO_pBullet->Deactivate();

    GO_pSniper->GetComponent<GCScriptSniper>()->SetBulletTemplate( GO_pBullet );
    GO_pMachineGun->GetComponent<GCScriptMachineGun>()->SetBulletTemplate( GO_pBullet );
    GO_pShotgun->GetComponent<GCScriptShotgun>()->SetBulletTemplate( GO_pBullet );

    GO_pSniper->Deactivate();
    GO_pMachineGun->Deactivate();
    GO_pShotgun->Deactivate();


    GCGameObject* GO_pExp = SC_pGame->CreateGameObject();
    GO_pExp->AddComponent<GCText>();
    GO_pExp->AddComponent<GCScriptExpText>()->SetPlayer( GO_pPlayer );
    GO_pExp->m_transform.SetPosition(GCVEC3(-4, 4, 0));

    GCGameObject* GO_pHp = SC_pGame->CreateGameObject();
    GO_pHp->AddComponent<GCText>();
    GO_pHp->AddComponent<GCScriptHpText>()->SetPlayer(GO_pPlayer);
    GO_pHp->m_transform.SetPosition(GCVEC3(-7, 4, 0));
    
#pragma endregion


#pragma region CARDSCENE
    GCScene* SC_pCards = GCScene::Create();
    GCGameObject* GO_cardTemplate = SC_pCards->CreateGameObject();
    GO_cardTemplate->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    GO_cardTemplate->AddComponent<GCButton>();
    GO_cardTemplate->AddComponent<GCScriptCard>()->SetGameScene(SC_pGame);
    GO_cardTemplate->GetComponent<GCScriptCard>()->SetPlayer(GO_pPlayer);
    GO_cardTemplate->Deactivate();
    
    SR_pPlayerScript->SetUpgradeScene( SC_pCards );
    SR_pPlayerScript->SetCardTemplate(GO_cardTemplate);

#pragma endregion

#pragma region SELECTWEAPONSCENE
    
    GCScene* SC_pWeaponSelect = GCScene::Create();

    GCGameObject* GO_pWeaponTitle = SC_pWeaponSelect->CreateGameObject();
    GO_pWeaponTitle->AddComponent<GCText>()->SetText(std::string("\bChoose   your   woolpon \b"));
    GO_pWeaponTitle->m_transform.SetPosition(GCVEC3(-2.5, 3, 0));

    GCGameObject* GO_pButtonSelect = SC_pWeaponSelect->CreateGameObject();
    GO_pButtonSelect->AddComponent<GCButton>();
    GCScriptButtonSelect* SR_pButtonSelect = GO_pButtonSelect->AddComponent<GCScriptButtonSelect>();
    SR_pButtonSelect->SetGameScene( SC_pGame );
    SR_pButtonSelect->SetPlayer( GO_pPlayer );
    GO_pButtonSelect->AddComponent<GCSpriteRenderer>();

    GCSprite SP_SelectSniper = GCSprite(std::string("SelectSniper.dds"));
    GCSprite SP_SelectSG = GCSprite(std::string("SelectShotgun.dds"));
    GCSprite SP_SelectMG = GCSprite(std::string("SelectMinigun.dds"));
    
    GCGameObject* GO_pSelectSniper = GO_pButtonSelect->Duplicate();
    GO_pSelectSniper->GetComponent<GCScriptButtonSelect>()->SetWeapon( 0 );
    GO_pSelectSniper->GetComponent<GCSpriteRenderer>()->SetSprite(&SP_SelectSniper);
    GO_pSelectSniper->m_transform.SetPosition( GCVEC3( -3.0f , 0.0f , 0.0f ) );
    GO_pSelectSniper->m_transform.SetScale( GCVEC3( 1.5f , 1.5f , 0.0f ) );

    GCGameObject* GO_pSelectSG = GO_pButtonSelect->Duplicate();
    GO_pSelectSG->GetComponent<GCScriptButtonSelect>()->SetWeapon( 1 );
    GO_pSelectSG->GetComponent<GCSpriteRenderer>()->SetSprite(&SP_SelectSG);
    GO_pSelectSG->m_transform.SetPosition(GCVEC3(0.0f, 0.0f, 0.0f));
    GO_pSelectSG->m_transform.SetScale(GCVEC3(1.5f, 1.5f, 0.0f));

    GCGameObject* GO_pSelectMG = GO_pButtonSelect->Duplicate();
    GO_pSelectMG->GetComponent<GCScriptButtonSelect>()->SetWeapon( 2 );
    GO_pSelectMG->GetComponent<GCSpriteRenderer>()->SetSprite(&SP_SelectMG);
    GO_pSelectMG->m_transform.SetPosition(GCVEC3(3.0f, 0.0f, 0.0f));
    GO_pSelectMG->m_transform.SetScale(GCVEC3(1.5f, 1.5f, 0.0f));

    GO_pButtonSelect->Deactivate();

#pragma endregion

#pragma region HOMESCENE

    GCScene* SC_pHome = GCScene::Create();
    SC_pHome->SetActive();

    GCSprite SP_Title = GCSprite(std::string("WITGN.dds"));

    GCGameObject* GO_pTitle = SC_pHome->CreateGameObject();
    GO_pTitle->m_transform.SetPosition(GCVEC3(-2.5, -1, 0));
    GO_pTitle->m_transform.SetScale(GCVEC3(5.5, 5.5, 0));
    GO_pTitle->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Title);
    GO_pTitle->SetLayer(2);

    //GCGameObject* GO_CreditLeft = SC_pHome->CreateGameObject();
    //GO_CreditLeft->m_transform.SetPosition(GCVEC3(-9, 4, 0));
    //GO_CreditLeft->AddComponent<GCText>()->SetText(std::string("\b Elia MAILLARD-SALIN \b"));

    //GCGameObject* GO_CreditRight = SC_pHome->CreateGameObject();
    //GO_CreditRight->m_transform.SetPosition(GCVEC3(4.5, 4, 0));
    //GO_CreditRight->AddComponent<GCText>()->SetText(std::string("\b Alexandre GLATZ \b"));

    GCSprite SP_Play = GCSprite(std::string("Play.dds"));
    GCSprite SP_Quit = GCSprite(std::string("QUIT.dds"));

    GCGameObject* GO_pMainPlayButton = SC_pHome->CreateGameObject();
    GO_pMainPlayButton->m_transform.SetPosition(GCVEC3(-4, -3, 0));
    GO_pMainPlayButton->m_transform.SetScale(GCVEC3(2.5, 1.59, 0));
    GO_pMainPlayButton->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Play);
    GO_pMainPlayButton->AddComponent<GCButton>();
    GO_pMainPlayButton->AddComponent<GCScriptButton>()->SetType(GCScriptButton::Play);
    GO_pMainPlayButton->GetComponent<GCScriptButton>()->SetPlayScene(SC_pWeaponSelect);
    GO_pMainPlayButton->SetLayer(2);

    GCGameObject* GO_pMainQuitButton = SC_pHome->CreateGameObject();
    GO_pMainQuitButton->m_transform.SetPosition(GCVEC3(2, -3, 0));
    GO_pMainQuitButton->m_transform.SetScale(GCVEC3(2.5, 1.59, 0));
    GO_pMainQuitButton->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Quit);
    GO_pMainQuitButton->AddComponent<GCButton>();
    GO_pMainQuitButton->AddComponent<GCScriptButton>()->SetType(GCScriptButton::Quit);
    GO_pMainQuitButton->SetLayer(2);


#pragma endregion

#pragma region PAUSESCENE

    GCGameObject* GO_pPlayButton = SC_pPause->CreateGameObject();
    GO_pPlayButton->m_transform.SetPosition(GCVEC3(-1.25, 1, 0));
    GO_pPlayButton->m_transform.SetScale(GCVEC3(2.5, 1.59, 0));
    GO_pPlayButton->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Play);
    GO_pPlayButton->AddComponent<GCButton>();
    GO_pPlayButton->AddComponent<GCScriptButton>()->SetType(GCScriptButton::Play);
    GO_pPlayButton->GetComponent<GCScriptButton>()->SetPlayScene(SC_pGame);


    GCGameObject* GO_pQuitButton = SC_pPause->CreateGameObject();
    GO_pQuitButton->m_transform.SetPosition(GCVEC3(-1.25, -1, 0));
    GO_pQuitButton->m_transform.SetScale(GCVEC3(2.5, 1.59, 0));
    GO_pQuitButton->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Quit);
    GO_pQuitButton->AddComponent<GCButton>();
    GO_pQuitButton->AddComponent<GCScriptButton>()->SetType(GCScriptButton::Quit);

#pragma endregion

    pGameManager->Run();
    return 0;
}