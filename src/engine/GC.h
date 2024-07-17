#pragma once
#include "GameManager.h"

class GCGameManager;
class GCPhysicManager;
class GCUpdateManager;
class GCEventManager;
class GCSceneManager;
class GCRenderManager;

class GC
{

private:
    GC() = delete;
    ~GC() = delete;

public:
    static GCGameManager* GetActiveGameManager();
    
    static GCPhysicManager* GetActivePhysicManager();
    static GCUpdateManager* GetActiveUpdateManager();
    static GCEventManager* GetActiveEventManager();
    static GCSceneManager* GetActiveSceneManager();
    // static GCRenderManager* GetActiveRenderManager();

private:
    inline static GCGameManager* m_pActiveGameManager = new GCGameManager(); //! To Change after the Game has been implemented

};