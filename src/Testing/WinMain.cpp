#include "pch.h"
#include "GCEngine.h"
#include "ScriptStart.h"


using namespace DirectX;

void Hello()
{
    std::cout << "Hello" << std::endl;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    GCGameObject* pMario = pScene->CreateGameObject();
    GCGameObject* pCAOW = pScene->CreateGameObject();
    //pCAOW->AddComponent<SpriteRenderer>()->SetSprite("caow.dds");
    //pCAOW->AddComponent<BoxCollider>()->SetVisible( true );
    //pMario->AddComponent<SpriteRenderer>()->SetSprite( "GoofyMario.dds" );
    //pMario->AddComponent<BoxCollider>()->SetVisible( true );
    pMario->m_transform.SetScale({ 5 , 5 , 0 });
    pCAOW->m_transform.SetScale({ 5,5,0 });
    ScriptStart* pScriptStart = pMario->AddComponent<ScriptStart>();
    //int myID = Event::CreateEventHandler(new EventHandler([]() { Hello(); }));
    //Event::CallEventHandler(myID);
    pGameManager->Run();
    

    // Initialisation des ressources graphiques
    //int flagsLightColor = 0;
    //SET_FLAG(flagsLightColor, HAS_POSITION);
    //SET_FLAG(flagsLightColor, HAS_COLOR);
    //SET_FLAG(flagsLightColor, HAS_NORMAL);

    //int flagsLightTexture = 0;
    //SET_FLAG(flagsLightTexture, HAS_POSITION);
    //SET_FLAG(flagsLightTexture, HAS_UV);
    //SET_FLAG(flagsLightTexture, HAS_NORMAL);

    //GCScene* pScene = GCScene::Create();

    //GCGameObject* test1 = pScene->CreateGameObject();
    //GCGameObject* test3 = pScene->CreateGameObject();
    //GCGameObject* test3bis = pScene->CreateGameObject();
    //GCGameObject* test2 = pScene->CreateGameObject();

    //test3->SetLayer(10);
    //test2->SetLayer(40);
    //test3bis->SetLayer(15);

    ////test1->AddComponent<BoxCollider>();
    //test2->AddComponent<BoxCollider>()->SetVisible(true);
    //test2->AddComponent<SpriteRenderer>()->SetSprite("caow.dds");

    //test3->AddComponent<BoxCollider>()->SetVisible(true);
    //test3->AddComponent<SpriteRenderer>()->SetSprite("Captain_Flameheart_Art.dds");

    //test3bis->AddComponent<SpriteRenderer>()->SetSprite("gojo.dds");

    //test1->m_transform.SetPosition(GCVEC3(0, 0, 0));
    //
    //test2->m_transform.SetPosition(GCVEC3(0, 0, 0));
    //test2->m_transform.SetScale(GCVEC3(5, 8.5, 1));

    //test3->m_transform.SetPosition(GCVEC3(0, 0, 0));
    //test3->m_transform.SetScale(GCVEC3(10, 10, 1));

    //test3bis->m_transform.SetPosition(GCVEC3(0, 0, 0));
    //test3bis->m_transform.SetScale(GCVEC3(2, 2, 1));

    //test2->SetLayer(0);

    ////auto startTime = std::chrono::steady_clock::now();
    //
    //int counter = 0;
    //GCInputSystem* pInputs = GC::GetActiveInputSystem();

    //pGameManager->Run();
    


    return 0;
}
