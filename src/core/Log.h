#pragma once
#include "pch.h"

//Engine Log Macros
#define LogEngineTrace(...) Logger::GetEngineLogger().Trace(__VA_ARGS__)
#define LogEngineDebug(...) Logger::GetEngineLogger().Debug(__VA_ARGS__)
#define LogEngineInfo(...) Logger::GetEngineLogger().Info(__VA_ARGS__)
#define LogEngineWarn(...) Logger::GetEngineLogger().Warn(__VA_ARGS__)
#define LogEngineError(...) Logger::GetEngineLogger().Error(__VA_ARGS__)


//Game Log Macros
#define LogGameTrace(...) Logger::GetGameLogger().Trace(__VA_ARGS__)
#define LogGameDebug(...) Logger::GetGameLogger().Debug(__VA_ARGS__)
#define LogGameInfo(...) Logger::GetGameLogger().Info(__VA_ARGS__)
#define LogGameWarn(...) Logger::GetGameLogger().Warn(__VA_ARGS__)
#define LogGameError(...) Logger::GetGameLogger().Error(__VA_ARGS__)
