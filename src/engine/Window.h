#pragma once
#include <Windows.h>
#include "EventSystem.h"

class GCWindow
{
public:
    GCWindow();
    ~GCWindow();

    bool Create();
    void Show();
    void PollEvents();

private:
	void EnableVirtualTerminalProcessing();

private:

};
