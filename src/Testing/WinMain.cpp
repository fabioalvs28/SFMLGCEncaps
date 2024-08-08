#include "pch.h"
#include "GCEngine.h"


int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    
    GCSprite pPlayerSprite( "goofyImage.dds" );
    GCGameObject* pPlayer = pScene->CreateGameObject();
    pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite( &pPlayerSprite );
    pPlayer->AddComponent<GCBoxCollider>()->SetVisible( false );
    GCAnimator* pAnimator = pPlayer->AddComponent<GCAnimator>();
    pAnimator->LoadSpriteSheet( "sprite_sheet.dds", 4, 6, 823, 823 );
    pAnimator->CreateAnimation( "walk", 0, 6, 0.1f );
    pAnimator->PlayAnimation( "walk", true );
    pPlayer->AddComponent<GCScriptPlayerMovement>();
    pPlayer->SetLayer(1);
    
    GCGameObject* pMainCamera = pScene->GetMainCamera()->GetGameObject();
    pPlayer->AddChild( pMainCamera );
    // pMainCamera->AddComponent<GCScriptFollowPlayer>()->SetPlayer( pPlayer );
    
    GCSprite pHappySprite( "happyImage.dds" );
    GCGameObject* pHappy1 = pScene->CreateGameObject();
    pHappy1->AddComponent<GCSpriteRenderer>()->SetSprite( &pHappySprite );
    pHappy1->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy1->m_transform.SetPosition( GCVEC3( 0.0f, 0.0f, 0.0f ) );
    
    GCGameObject* pHappy2 = pScene->CreateGameObject();
    pHappy2->AddComponent<GCSpriteRenderer>()->SetSprite( &pHappySprite );
    pHappy2->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy2->m_transform.SetPosition( GCVEC3( 0.0f, 2.1f, 0.0f ) );
    
    GCGameObject* pHappy3 = pScene->CreateGameObject();
    pHappy3->AddComponent<GCSpriteRenderer>()->SetSprite( &pHappySprite );
    pHappy3->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy3->m_transform.SetPosition( GCVEC3( 0.0f, -2.1f, 0.0f ) );
    
    pGameManager->Run();
    
    return 0;
}