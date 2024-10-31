#pragma once
#include "pch.h"


//Core Log Macros
#define LogCoreTrace(...) Logger::GetCoreLogger().Trace(__VA_ARGS__)
#define LogCoreDebug(...) Logger::GetCoreLogger().Debug(__VA_ARGS__)
#define LogCoreInfo(...) Logger::GetCoreLogger().Info(__VA_ARGS__)
#define LogCoreWarn(...) Logger::GetCoreLogger().Warn(__VA_ARGS__)
#define LogCoreError(...) Logger::GetCoreLogger().Error(__VA_ARGS__)


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
