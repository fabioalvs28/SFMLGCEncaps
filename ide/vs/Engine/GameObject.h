#pragma once
#include <map>
#include "Components.h"



class GameObject
{

public:
    GameObject();
    ~GameObject();
    
    void Update();

protected:
    static inline int s_nextID = 0;
    int m_ID;
    const char* m_name;
    bool m_active;
    const char* m_tag;
    int m_layer;
    std::map<int, Component*> m_componentsList;

};

