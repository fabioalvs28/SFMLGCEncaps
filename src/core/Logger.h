#pragma once

#include <cstdio>
#include <ctime>
#include <mutex>
#include <stdarg.h>


//REMOVE WARNING FROM VISUAL STUDIO
#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS
#endif

class Logger
{
public:

	enum LogPriority
	{
		LOG_TRACE,
		LOG_DEBUG,
		LOG_INFO,
		LOG_WARN,
		LOG_ERROR,
		LOG_FATAL
	};

	static Logger& GetCoreLogger()
	{
		static Logger coreLogger("Core: ");
		return coreLogger;
	}

	static Logger& GetEngineLogger()
	{
		static Logger engineLogger("Engine: ");
		return engineLogger;
	}

	static Logger& GetGameLogger()
	{
		static Logger gameLogger("Game: ");
		return gameLogger;
	}


	void SetLogPriority(LogPriority priority) { m_logPriority = priority; };
	const LogPriority GetLogPriority() const { return m_logPriority; };

	template<typename ...Args>
	void Trace(const char* message, Args&& ... args)
	{
		Log(LOG_TRACE, message, args ...);
	}

	template<typename ...Args>
	void Debug(const char* message, Args&& ... args)
	{
		Log(LOG_DEBUG, message, args ...);
	}

	template<typename ...Args>
	void Info(const char* message, Args&& ... args)
	{
		Log(LOG_INFO, message, args ...);
	}

	template<typename ...Args>
	void Warn(const char* message, Args&& ... args)
	{
		Log(LOG_WARN, message, args ...);
	}

	template<typename ...Args>
	void Error(const char* message, Args&& ... args)
	{
		Log(LOG_ERROR, message, args ...);
	}

private:
	// Private constructor to prevent instancing
	Logger(const char* name);


	const char* MessagePriorityToString(LogPriority priority);
	void SetColor(LogPriority priority);

	template<typename ...Args>
	void Log(LogPriority msgPriority, const char* message, Args&& ... args);


	LogPriority m_logPriority = LOG_DEBUG;
	std::mutex m_LogLock;
	const char* m_InitialString = "";
	const char* m_TimestampFormat = "%Y-%m-%d %H:%M:%S";
	char m_TimeBuffer[80];

	static const char* NormalColorCode;
	static const char* WhiteColorCode;
	static const char* GreenColorCode;
	static const char* YellowColorCode;
	static const char* RedColorCode;
};

template<typename ...Args>
void Logger::Log(LogPriority msgPriority, const char* message, Args&& ... args)
{
	if (m_logPriority <= msgPriority)
	{
		std::time_t currentTime = std::time(nullptr);
		std::tm timestamp;
		localtime_s(&timestamp, &currentTime);

		std::scoped_lock lock(m_LogLock);

		SetColor(msgPriority);
		std::strftime(m_TimeBuffer, sizeof(m_TimeBuffer), m_TimestampFormat, &timestamp);
		std::printf("%s ", m_TimeBuffer);

		const char* msgPriorityString = MessagePriorityToString(msgPriority);
		std::printf("%s%s", msgPriorityString, m_InitialString);

		std::printf(message, args ...);
		std::printf("\n");

		std::printf(NormalColorCode);
	}
}