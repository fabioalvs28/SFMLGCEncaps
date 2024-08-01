#include "pch.h"
#include "GCEngine.h"
#include "ScriptStart.h"



using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    // Create the GameManager
    GCGameManager* pGameManager = GC::CreateGameManager(hInstance);
    GCScene::Create()->AddScript<ScriptStart>();

    // Start the game
    pGameManager->Run();

    return 0;
}
