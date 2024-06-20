#include "pch.h"

#include <iostream>
#include <Windows.h>
#include "Window.h"
#include "EventSystem.h"
#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "GCColor.h"
#include "Log.h"




int main()
{
	LogEngineDebug("Engine Debug");
    LogEngineError("Engine Error");
	LogEngineWarn("Engine Warn");
    LogEngineInfo("Engine Info");
	LogEngineTrace("Engine Trace");
	LogGameDebug("Game Debug");
}