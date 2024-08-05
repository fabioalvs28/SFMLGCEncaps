#include "pch.h"
#include "GCEngine.h"

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Create the GameManager
    GCGameManager* pGameManager = GC::CreateGameManager(hInstance);
    
    // Create a Scene
    GCScene* pScene = GCScene::Create();
    
    // Create a GameObject
    GCGameObject* pGameObject = pScene->CreateGameObject();
    
    // Start the game
    pGameManager->Run();
    
    return 0;
}