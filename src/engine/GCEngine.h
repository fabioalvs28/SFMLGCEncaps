#pragma once

#include "GCWindow.h"

/// <summary>
/// This class is different from Engine.h
/// This class is the application class in the engine
/// </summary>
class GCEngine
{
public:
    void Shutdown();
    void Run();

    void OnEvent(GCEvent& ev);

private:
	GCEngine();
	~GCEngine();

	bool InitWindow();
    
    //TODO: Initialize D3D12 here
    bool InitD3D12();

    void Cleanup();

private:
    GCWindow* m_window = nullptr;
};
