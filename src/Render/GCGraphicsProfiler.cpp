#include "framework.h"

GCGraphicsProfiler* GCGraphicsProfiler::s_instance = nullptr;

GCGraphicsProfiler::GCGraphicsProfiler() 
{
    InitializeConsole();
}

GCGraphicsProfiler::~GCGraphicsProfiler() 
{
    FreeConsole();
}

GCGraphicsProfiler& GCGraphicsProfiler::GetInstance() 
{
    if (!s_instance) 
    {
        s_instance = new GCGraphicsProfiler();
    }
    return *s_instance;
}

void GCGraphicsProfiler::Log(const std::string& message)
{
    std::cout << "" << message << std::endl;
}

void GCGraphicsProfiler::LogWarning(const std::string& message) 
{
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << "[WARNING] " << message << std::endl;
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to white
}

void GCGraphicsProfiler::LogInfo(const std::string& message)
{
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[INFO] " << message << std::endl;
    SetConsoleTextAttribute(m_consoleHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset to white
}

void GCGraphicsProfiler::InitializeConsole() 
{
    AllocConsole();
    m_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
}
