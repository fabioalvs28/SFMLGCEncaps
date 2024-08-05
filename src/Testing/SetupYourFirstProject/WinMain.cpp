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
    
    // Add a sprite to the GameObject
    pGameObject->AddComponent<SpriteRenderer>()->SetSprite("goofyImage.dds");
    
    // Start the game
    pGameManager->Run();
    
    return 0;
}