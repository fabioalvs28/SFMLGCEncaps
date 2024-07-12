#pragma once

class GCGraphicsLogger 
{
public:
    static GCGraphicsLogger& GetInstance();

    void InitializeConsole();

    void Log(const std::string& message);
    void LogWarning(const std::string& message);
    void LogInfo(const std::string& message);

private:
    GCGraphicsLogger();
    ~GCGraphicsLogger();

    GCGraphicsLogger(const GCGraphicsLogger&) = delete;
    GCGraphicsLogger& operator=(const GCGraphicsLogger&) = delete;

    static GCGraphicsLogger* s_instance;

    HANDLE m_consoleHandle;
};

