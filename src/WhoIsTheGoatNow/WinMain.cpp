#include "pch.h"
#include "GCEngine.h"
#include "PlayerBehaviour.h"


int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    
    GCSprite SP_pPlayer("blue_square.dds");
    GCGameObject* GO_pPlayer = pScene->CreateGameObject();
    GO_pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite(&SP_pPlayer);
    GO_pPlayer->AddComponent<GCBoxCollider>();
    GO_pPlayer->AddComponent<GCScriptPlayerBehaviour>();
    GO_pPlayer->SetLayer(1);

    pGameManager->Run();
    
    return 0;
}