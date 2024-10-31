#pragma once
#include "pch.h"

class GCUpdateManager
{
friend class GCComponent;
friend class GCGameManager;

private:
    GCUpdateManager() = default;
    ~GCUpdateManager() = default;
    
    void Update();
    
    void RegisterComponent( GCComponent* pComponent );

private:
    GCList<GCComponent*> m_registeredComponents; // A list of pointers to the registered Components that wants to be updated.

};