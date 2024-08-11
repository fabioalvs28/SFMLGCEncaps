#include "pch.h"
#include "GCEngine.h"


int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
  
    pGameManager->Run();
    
    return 0;
}