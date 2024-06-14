#pragma once

class GCGraphicsProfiler {
public:
    static GCGraphicsProfiler& GetInstance();

    void InitializeConsole();

    void Log(const std::string& message);
    void LogWarning(const std::string& message);
    void LogInfo(const std::string& message);

private:
    GCGraphicsProfiler();
    ~GCGraphicsProfiler();



    GCGraphicsProfiler(const GCGraphicsProfiler&) = delete;
    GCGraphicsProfiler& operator=(const GCGraphicsProfiler&) = delete;

    static GCGraphicsProfiler* s_instance;

    HANDLE m_consoleHandle;
};

