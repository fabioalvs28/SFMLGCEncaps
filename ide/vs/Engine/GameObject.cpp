#include "pch.h"
#include "GameObject.h"
#include "Components.h"



GCGameObject::GCGameObject()
{
    m_ID = s_nextID++;
    m_name = "";
    m_active = true;
    m_tag = "";
    m_layer = 0;
}

void GCGameObject::Init( const char* name, bool active )
{
    m_name = name;
    m_active = active;
}

// TODO Update Every Component

// TODO Destroy GameObject + Remove it from it's list