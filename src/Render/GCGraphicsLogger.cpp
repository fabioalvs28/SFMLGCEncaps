#include "pch.h"

GCGraphicsLogger* GCGraphicsLogger::s_instance = nullptr;

GCGraphicsLogger::GCGraphicsLogger() 
{
    InitializeConsole();
}

GCGraphicsLogger::~GCGraphicsLogger() 
{
    FreeConsole();
}

GCGraphicsLogger& GCGraphicsLogger::GetInstance() 
{
    if (!s_instance) 
    {
        s_instance = new GCGraphicsLogger();
    }
    return *s_instance;
}

void GCGraphicsLogger::Log(const std::string& message)
{
    std::cout << "" << message << std::endl;
}

void GCGraphicsLogger::LogWarning(const std::string& message) 
{
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << "[WARNING] " << message << std::endl;
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to white
}

void GCGraphicsLogger::LogInfo(const std::string& message)
{
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[INFO] " << message << std::endl;
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to white
}

void GCGraphicsLogger::InitializeConsole() 
{
    AllocConsole();
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
}
