#include "pch.h"
#include "GCEngine.h"

#include "PlayerMovement.h"
#include "FollowPlayer.h"

int WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();
    
    GCGameObject* pPlayer = pScene->CreateGameObject();
    pPlayer->AddComponent<GCSpriteRenderer>()->SetSprite( "goofyImage.dds" );
    pPlayer->AddComponent<GCBoxCollider>()->SetVisible( false );
    pPlayer->AddComponent<GCScriptPlayerMovement>();
    
    GCGameObject* pMainCamera = pScene->FindGameObjectByName( "MainCamera" );
    pPlayer->AddChild( pMainCamera );
    // pMainCamera->AddComponent<GCScriptFollowPlayer>()->SetPlayer( pPlayer );
    
    GCGameObject* pHappy1 = pScene->CreateGameObject();
    pHappy1->AddComponent<GCSpriteRenderer>()->SetSprite( "happyImage.dds" );
    pHappy1->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy1->m_transform.SetPosition( GCVEC3( 0.0f, 0.0f, 0.0f ) );
    
    GCGameObject* pHappy2 = pScene->CreateGameObject();
    pHappy2->AddComponent<GCSpriteRenderer>()->SetSprite( "happyImage.dds" );
    pHappy2->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy2->m_transform.SetPosition( GCVEC3( 0.0f, 2.1f, 0.0f ) );
    
    GCGameObject* pHappy3 = pScene->CreateGameObject();
    pHappy3->AddComponent<GCSpriteRenderer>()->SetSprite( "happyImage.dds" );
    pHappy3->AddComponent<GCBoxCollider>()->SetVisible( false );
    pHappy3->m_transform.SetPosition( GCVEC3( 0.0f, -2.1f, 0.0f ) );
    
    pGameManager->Run();
    
    return 0;
}