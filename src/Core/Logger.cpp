#include "pch.h"

const char* Logger::s_normalColorCode = "\x1B[0m";
const char* Logger::s_whiteColorCode = "\x1B[37m";
const char* Logger::s_greenColorCode = "\x1B[32m";
const char* Logger::s_yellowColorCode = "\x1B[33m";
const char* Logger::s_redColorCode = "\x1B[31m";


Logger::Logger(const char* name)
{
	m_initialString = name;
}

const char* Logger::MessagePriorityToString(LogPriority priority)
{
	const char* priorityString;

	switch (priority)
	{
	case Logger::LOG_TRACE: priorityString = "[TRACE] "; break;
	case Logger::LOG_DEBUG: priorityString = "[DEBUG] "; break;
	case Logger::LOG_INFO: priorityString = "[INFO] "; break;
	case Logger::LOG_WARN: priorityString = "[WARN] "; break;
	case Logger::LOG_ERROR: priorityString = "[ERROR] "; break;
	default: priorityString = "[PRIORITY] "; break;
	}

	return priorityString;
}

void Logger::SetColor(LogPriority priority)
{
	const char* color;

	switch (priority)
	{

	case Logger::LOG_TRACE: color = s_whiteColorCode; break;
	case Logger::LOG_DEBUG: color = s_whiteColorCode; break;
	case Logger::LOG_INFO: color = s_greenColorCode; break;
	case Logger::LOG_WARN: color = s_yellowColorCode; break;
	case Logger::LOG_ERROR: color = s_redColorCode; break;
	default: color = s_normalColorCode; break;
	}

	std::printf("%s", color);
}
