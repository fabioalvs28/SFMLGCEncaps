#pragma once
#include "GameManager.h"

class Component;
class Collider;
class GCGameObject;
class GCScene;
class GCPhysicManager;

class GC
{
friend class Collider;
friend class Component;
friend class GCGameObject;
friend class GCScene;
friend class GCPhysicManager;

private:
    GC() = delete;
    ~GC() = delete;

private:
public: static inline GCGameManager m_pActiveGameManager = GCGameManager();

};