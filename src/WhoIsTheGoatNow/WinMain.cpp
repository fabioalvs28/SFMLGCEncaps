#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"
#include "EnemyBehaviour.h"


int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    
    //Player creation
    GCSprite SP_pPlayer("blue_square.dds");

    GCGameObject* GO_pPlayer = pScene->CreateGameObject();
    GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_pPlayer);
    GO_pPlayer->AddComponent<GCBoxCollider>();
    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>();
    GO_pPlayer->SetLayer(1);

    //Enemy creation
    GCSprite SP_pEnemy("red_square.dds");

    GCGameObject* GO_pEnemy = pScene->CreateGameObject();
    GO_pEnemy->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_pEnemy);
    GO_pEnemy->AddComponent<GCBoxCollider>();
    GO_pEnemy->AddComponent<GCScriptEnemyBehaviour>()->SetTarget(GO_pPlayer);

    //GO_pEnemy->GetComponent<GCScriptEnemyBehaviour>()->Spawn();
    GO_pPlayer->SetLayer(1);
    pGameManager->Run();
    
    return 0;
}