#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"
#include "EnemyBehaviour.h"
#include "EnemySpawner.h"
#include "Bullet.h"
#include "Test.h";

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    
    //GCGameObject* ptest = pScene->CreateGameObject();
    //ptest->AddComponent<GCScriptTest>();

    //Bullet prefab 
    GCSprite SP_bullet("black_small_square.dds");

    GCGameObject* GO_pBullet = pScene->CreateGameObject();
    GO_pBullet->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_bullet);
    GO_pBullet->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pBullet->AddComponent<GCScriptBullet>();
    GO_pBullet->AddTag("bullet");
    GO_pBullet->m_transform.Scale(0.5f);
    GO_pBullet->Deactivate();

    //Player creation
    GCSprite SP_player("blue_square.dds");

    GCGameObject* GO_pPlayer = pScene->CreateGameObject();
    GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_player);
    GO_pPlayer->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>()->SetBulletTemplate(GO_pBullet);
    GO_pPlayer->AddTag("player");
    GO_pPlayer->SetLayer(1);

    GO_pBullet->GetComponent<GCScriptBullet>()->SetOrigin(GO_pPlayer);
    //Enemy prefab
    GCSprite SP_enemy("red_square.dds");

    GCGameObject* GO_pEnemy = pScene->CreateGameObject();
    GO_pEnemy->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_enemy);
    GO_pEnemy->AddComponent<GCBoxCollider>()->SetVisible(true);
    GO_pEnemy->AddComponent<GCScriptEnemyBehaviour>()->SetTarget(GO_pPlayer);
    GO_pEnemy->AddTag("enemy");
    GO_pEnemy->Deactivate();

    GCGameObject* GO_pEnemySpawner = pScene->CreateGameObject();
    GO_pEnemySpawner->AddComponent<GCScriptEnemySpawner>()->SetTemplate(GO_pEnemy);

    
    pGameManager->Run();

    return 0;
}