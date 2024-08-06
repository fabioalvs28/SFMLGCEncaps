#pragma once
#include "pch.h"

class Layer
{
public:
    virtual void OnEvent( GCEvent& event ) = 0;
    virtual ~Layer() = default;
};

class UILayer : public Layer
{
public:
    void OnEvent( GCEvent& event ) override {} //TODO: use eventdispatcher to handle UI event

};
