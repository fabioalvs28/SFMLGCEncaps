#include "pch.h"
#include "GC.h"

#include "GameObject.h"
#include "Scene.h"



void GC::Destroy(GCGameObject*& pGameObject)
{
    pGameObject->Destroy();
    pGameObject = nullptr;
}

void GC::Destroy(GCScene*& pScene)
{
    pScene->Destroy();
    pScene = nullptr;
}



GCGameManager* GC::GetActiveGameManager()
{
    return m_pActiveGameManager;
}

GCPhysicManager* GC::GetActivePhysicManager()
{
    return &m_pActiveGameManager->m_pPhysicManager;
}

GCUpdateManager* GC::GetActiveUpdateManager()
{
    return &m_pActiveGameManager->m_pUpdateManager;
}

GCEventManager* GC::GetActiveEventManager()
{
    return &m_pActiveGameManager->m_pEventManager;
}

GCSceneManager* GC::GetActiveSceneManager()
{
    return &m_pActiveGameManager->m_pSceneManager;
}

GCRenderManager* GC::GetActiveRenderManager()
{
    return &m_pActiveGameManager->m_pRenderManager;
}


GCInputSystem* GC::GetActiveInputSystem()
{
    return &m_pActiveGameManager->m_pInputSystem;
}

GCTime* GC::GetActiveTimer()
{
    return &m_pActiveGameManager->m_Time;
}
