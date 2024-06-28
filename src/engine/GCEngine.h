#pragma once

#include "GCWindow.h"
#include "GameManager.h"

/// <summary>
/// This class is different from Engine.h
/// This class is the application class in the engine
/// </summary>
class GCEngine 
{
public:
    static GCEngine& Get()
    {
        static GCEngine instance;
        return instance;
    }

    void Shutdown();
    void Run();

    void OnEvent(GCEvent& ev);

private:
    GCEngine();
	~GCEngine();

	bool InitWindow();

    bool InitEngine();
    
    //TODO: Initialize D3D12 here
    bool InitD3D12();

    void Cleanup();

private:
    GCWindow* m_window = nullptr;
    GCGameManager* m_gameManager = nullptr;

    bool m_isRunning = false;
};
