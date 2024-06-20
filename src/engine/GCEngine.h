#pragma once

#include "GCWindow.h"
//This class is different from Engine.h

//This class plays the role of the application class in the engine
class GCEngine
{
public:
	GCEngine();
	~GCEngine();

    void Shutdown();
    void Run();

    void OnEvent(GCEvent& ev);

private:
	bool InitWindow();
    
    //TODO: Initialize D3D12 here
    bool InitD3D12();

private:
    GCWindow* m_window = nullptr;
};
