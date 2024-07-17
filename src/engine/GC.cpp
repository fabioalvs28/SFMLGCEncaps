#include "pch.h"
#include "GC.h"



GCGameManager* GC::GetActiveGameManager()
{ return m_pActiveGameManager; }

GCPhysicManager* GC::GetActivePhysicManager()
{ return &m_pActiveGameManager->m_pPhysicManager; }

GCUpdateManager* GC::GetActiveUpdateManager()
{ return &m_pActiveGameManager->m_pUpdateManager; }

GCEventManager* GC::GetActiveEventManager()
{ return &m_pActiveGameManager->m_pEventManager; }

GCSceneManager* GC::GetActiveSceneManager()
{ return &m_pActiveGameManager->m_pSceneManager; }

// GCRenderManager* GC::GetActiveRenderManager()
// { return &m_pActiveGameManager->m_pRenderManager; }