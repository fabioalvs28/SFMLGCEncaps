#pragma once
#include "pch.h"

class Layer
{
public:
    virtual void OnEvent(GCEvent& e) = 0;
    virtual ~Layer() = default;
};

class UILayer : public Layer
{
public:
    void OnEvent(GCEvent& e) override
    {
        //TODO: use eventdispatcher to handle UI event
    }

};
