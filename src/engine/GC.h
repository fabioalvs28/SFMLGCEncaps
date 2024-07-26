#pragma once
#include "List.h"
#include "GameManager.h"

class GCGameManager;
class GCPhysicManager;
class GCUpdateManager;
class GCEventManager;
class GCSceneManager;
class GCRenderManager;
struct HINSTANCE__;
typedef struct HINSTANCE__ *HINSTANCE;

class GC
{
    friend class GCGameManager;

private:
    GC() = delete;
    ~GC() = delete;

public:
    static GCGameManager* CreateGameManager( HINSTANCE hInstance );

    static GCGameManager* GetActiveGameManager();
    static Window* GetWindow();
    static GCInputSystem* GetActiveInputSystem();
    static GCEventManager* GetActiveEventManager();
    static GCPhysicManager* GetActivePhysicManager();
    static GCUpdateManager* GetActiveUpdateManager();
    static GCSceneManager* GetActiveSceneManager();
    static GCRenderManager* GetActiveRenderManager();
    static GCInputSystem* GetActiveInputSystem();
	static GCTime* GetActiveTimer();

private:
    inline static GCList<GCGameManager*> m_pGameManagersList = GCList<GCGameManager*>();
    inline static GCGameManager* m_pActiveGameManager = nullptr;

};