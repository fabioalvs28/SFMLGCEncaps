#include "pch.h"
#include "GameObject.h"



GameObject::GameObject()
{
    m_ID = s_nextID++;
    m_name = "GameObject";
    m_active = true;
    m_tag = "";
    m_layer = 0;
}

void GameObject::Update()
{
    for ( int i = 0; i < m_componentsList.size(); i++ )
        m_componentsList[i]->Update();
}