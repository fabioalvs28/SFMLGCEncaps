#pragma once
#include "List.h"

class Component;

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
    GCList<Component*> m_registeredComponents;

};