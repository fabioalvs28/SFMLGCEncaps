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

GCGameObject::GCGameObject( const char* name, bool active )
{
    m_ID = s_nextID++;
    m_name = name;
    m_active = active;
    m_tag = "";
    m_layer = 0;
}

GCGameObject::GCGameObject( const char* name, bool active, const char* tag, int layer ) 
{
    m_ID = s_nextID++;
    m_name = name;
    m_active = active;
    m_tag = tag;
    m_layer = layer;
}

void GCGameObject::Update()
{
    Component* component; 
    for (int i = 1 ; i <= 7 ; i++)
        if ( m_componentsList.Find(i, component) == true ) 
            component->Update();
}

