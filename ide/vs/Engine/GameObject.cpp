#include "pch.h"
#include "GameObject.h"



GameObject::GameObject()
{
    m_ID = s_nextID++;
    m_name = "";
    m_active = true;
    m_tag = "";
    m_layer = 0;
}

void GameObject::Init( const char* name, bool active )
{
    m_name = name;
    m_active = active;
}

void GameObject::Update()
{
    for ( int i = 0; i < m_componentsList.size(); i++ )
        m_componentsList[i]->Update();
}