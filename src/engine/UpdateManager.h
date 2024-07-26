#pragma once
#include "pch.h"
#include "../core/pch.h"

class GCUpdateManager
{
friend class Component;
friend class GCGameManager;

private:
    GCUpdateManager() = default;
    ~GCUpdateManager() = default;
    
    void Update();
    
    void RegisterComponent( Component* pComponent );

private:
    GCList<Component*> m_registeredComponents; // A list of pointers to the registered Components that wants to be updated.

};