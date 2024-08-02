#include "pch.h"
#include "GCEngine.h"
#include "ScriptStart.h"



using namespace DirectX;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    GCGameObject* pMario = pScene->CreateGameObject();
    GCGameObject* pCAOW = pScene->CreateGameObject();

    //Animated Character Test
    GCGameObject* pHero = pScene->CreateGameObject();
    pHero->m_transform.Scale( GCVEC3(3 , 3 , 3) );
    pHero->AddComponent<SpriteRenderer>();
    pHero->AddComponent<Animator>()->LoadSpriteSheet( "sprite_sheet.dds" , 4 , 6 , 823 , 823 );
    pHero->AddComponent<ScriptStart>();
    pHero->GetComponent<Animator>()->CreateAnimation( "WalkForward" , 0 , 6 , 1.0f );
    pHero->GetComponent<Animator>()->CreateAnimation( "WalkHeroL" , 6 , 6 , 0.01f );
    pHero->GetComponent<Animator>()->CreateAnimation( "WalkHeroR" , 12 , 6 , 0.01f );
    pHero->GetComponent<Animator>()->CreateAnimation( "WalkBackward" , 18 , 6 , 0.01f );


    GCGameObject* pCopy = pScene->CreateGameObject();
    pCopy->AddComponent<SpriteRenderer>();
    pCopy->AddComponent<Animator>()->PlayAnimation( "WalkForward" );
    pCopy->m_transform.Scale( GCVEC3( 3 , 3 , 3 ) );
    pHero->AddComponent<BoxCollider>()->SetVisible( true );


    // Start the game
    pGameManager->Run();

    return 0;
}
