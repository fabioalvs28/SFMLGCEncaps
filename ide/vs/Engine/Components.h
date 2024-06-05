#pragma once



class Component
{

public:
    virtual ~Component() {};
    
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Destroy() = 0;

};

