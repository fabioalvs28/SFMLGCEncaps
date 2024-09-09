#pragma once
#include "pch.h"

class Window;
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
    static GCTime* GetActiveTimer();
    static GCInputSystem* GetActiveInputSystem();
    static GCEventManager* GetActiveEventManager();
    static GCPhysicManager* GetActivePhysicManager();
    static GCUpdateManager* GetActiveUpdateManager();
    static GCSceneManager* GetActiveSceneManager();
    static GCScene* GetActiveScene();
    static GCRenderManager* GetActiveRenderManager();
    static GCSpriteSheetHandler* GetActiveSpriteSheetHandler();
    static GCTextManager* GetActiveTextManager();

private:
    inline static GCList<GCGameManager*> m_gameManagersList = GCList<GCGameManager*>();
    inline static GCGameManager* m_pActiveGameManager = nullptr;

};