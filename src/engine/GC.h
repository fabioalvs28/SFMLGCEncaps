#pragma once
#include "GameManager.h"

class GCGameObject;
class GCScene;

class GC
{
friend class GCGameObject;
friend class GCScene;

private:
    GC() = delete;
    ~GC() = delete;

private:
    static inline GCGameManager m_pActiveGameManager = GCGameManager();

};