#pragma once
#include <map>
#include "Components.h"



class GameObject
{

public:
    GameObject();
    ~GameObject();
    
    void Update();
    template<class T>
    T* AddComponent();
    template<class T>
    T* GetComponent();

protected:
    static inline int s_nextID = 0;
    int m_ID;
    const char* m_name;
    bool m_active;
    const char* m_tag;
    int m_layer;
    std::map<int, Component*> m_componentsList;

};

template<class T>
T* GameObject::AddComponent()
{
    T* component = new T();
    m_componentsList[ component->GetID() ] = component;
    return component;
}

template<class T>
T* GameObject::GetComponent()
{
    return (T*) m_componentsList[]
}