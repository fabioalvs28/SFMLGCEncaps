#include "pch.h"
#include "GCEngine.h"
#include "ScriptStart.h"


using namespace DirectX;
///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Custom Event Example/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int heroHP = 100;

class TakeDamage : public CustomEvent<int> {
public:
    TakeDamage(int x) : m_upcomingDamage(x), CustomEvent(x) {}

    static GCEventType GetStaticType() { return GCEventType::Custom; }
    GCEventType GetEventType() { return GetStaticType(); }
    const char* GetName() const override { return "Damage Event"; }

    int GetDamage() { return m_upcomingDamage; }
private:
    int m_upcomingDamage;
};

void Hello()
{
    std::cout << "Hello" << std::endl;
    GC::GetActiveEventManager()->QueueEvent(new TakeDamage(10));
}

void OnDamageTaken(TakeDamage& dm) 
{
    heroHP -= dm.GetDamage();
    std::cout << heroHP << std::endl;
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    
    GCGameManager* pGameManager = GC::CreateGameManager( hInstance );
    GCScene* pScene = GCScene::Create();

    //int myID = EVENT::CreateEventHandler(new EventHandler(Hello));
    //EVENT::CallEventHandler(myID);
    //EVENT::CallEventHandler(myID);
    //EVENT::Subscribe(GCEventType::Custom, OnDamageTaken);
    //GCINPUT::BindKey(KEYBOARD::SPACE, GCKeyboardInputManager::KeyboardState::DOWN, Hello);
    //EVENT::PushEvent(new TakeDamage(10));

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
