#include "pch.h"

const char* Logger::NormalColorCode = "\x1B[0m";
const char* Logger::WhiteColorCode = "\x1B[37m";
const char* Logger::GreenColorCode = "\x1B[32m";
const char* Logger::YellowColorCode = "\x1B[33m";
const char* Logger::RedColorCode = "\x1B[31m";


Logger::Logger(const char* name)
{
	m_InitialString = name;
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

	case Logger::LOG_TRACE: color = WhiteColorCode; break;
	case Logger::LOG_DEBUG: color = WhiteColorCode; break;
	case Logger::LOG_INFO: color = GreenColorCode; break;
	case Logger::LOG_WARN: color = YellowColorCode; break;
	case Logger::LOG_ERROR: color = RedColorCode; break;
	default: color = NormalColorCode; break;
	}

	std::printf("%s", color);
}
