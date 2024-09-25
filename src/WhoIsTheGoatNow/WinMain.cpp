#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Bullet.h"
#include "Test.h";
#include "Mouse.h"
#include "Weapon.h"

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );

#pragma region GAMESCENE
    GCScene* pScene = GCScene::Create();
    
    GCSprite SP_enemy("red_square.dds");

    //GCGameObject* ptest = pScene->CreateGameObject();
    //ptest->AddComponent<GCText>();
    //ptest->AddComponent<GCScriptTest>()->pText = ptest->GetComponent<GCText>();
    //ptest->m_transform.SetPosition(GCVEC3(-4, 3, 0));

    //GCGameObject* GO_pGameScene = pScene->CreateGameObject();

    //Bullet prefab 
    GCSprite SP_bullet("black_small_square.dds");

    GCGameObject* GO_pBullet = pScene->CreateGameObject();
    GO_pBullet->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_bullet);
    //GO_pBullet->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pBullet->AddComponent<GCScriptBullet>();
    GO_pBullet->AddTag("bullet");
    GO_pBullet->m_transform.Scale(0.5f);
    GO_pBullet->Deactivate();

    //Player creation
    GCSprite SP_player("blue_square.dds");

    GCGameObject* GO_pPlayer = pScene->CreateGameObject();
    GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_player);
    GCAnimator* pAnimator = GO_pPlayer->AddComponent<GCAnimator>();
    pAnimator->LoadSpriteSheet("spritesheet_0.dds", 0);
    pAnimator->CreateAnimation("aaa", 0, 5, 0.3f);
    pAnimator->PlayAnimation("aaa",true);
    //GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_player);
    GO_pPlayer->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>()->SetBulletTemplate(GO_pBullet);
    GO_pPlayer->AddTag("player");
    GO_pPlayer->SetLayer(1);

    GO_pBullet->GetComponent<GCScriptBullet>()->SetOrigin(GO_pPlayer);

    //Enemy prefab

    GCGameObject* GO_pEnemy = pScene->CreateGameObject();
    GO_pEnemy->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    //dGO_pEnemy->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pEnemy->AddComponent<GCScriptEnemyBehaviour>()->SetTarget(GO_pPlayer);
    GO_pEnemy->AddTag("enemy");
    GO_pEnemy->Deactivate();

    //GCGameObject* GO_pEnemySpawner = pScene->CreateGameObject();
    //GO_pEnemySpawner->AddComponent<GCScriptEnemySpawner>()->SetTemplate(GO_pEnemy);

    GCSprite SP_Weapon("Shotgun.dds");
    GCGameObject* GO_pWeapon = GO_pPlayer->CreateChild();
    GO_pWeapon->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_Weapon);
    GO_pWeapon->AddComponent<GCScriptWeapon>();
    GO_pWeapon->AddComponent<GCText>()->SetText(std::to_string(GCINPUTS::GetMousePos().x));

    
#pragma endregion

#pragma region HOMESCENE

    GCScene* pHomeScene = GCScene::Create();
    pHomeScene->SetActive();

    GCGameObject* GO_Title = pHomeScene->CreateGameObject();
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
    pGameManager->Run();
    return 0;
}