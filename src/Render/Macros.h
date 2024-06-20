#pragma once

bool CheckHResult(HRESULT hr, const std::string& msg);

template<class T>
bool CheckNull(T value)
{

	if (value == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template<class T, class ...Args>
bool CheckNull(T value, Args ... args)
{

	if (value == nullptr)
	{
		return true;
	}
	else 
	{
		return CheckNull(args...);
	}
}

// Variadic Check Ptr with 1 message Error 
#define CHECK_POINTERSNULL(profiler, successMsg, warningMsg, ...) \
    do { \
        bool allNotNull = true; \
        const char* successMsgStr = successMsg; \
        const char* warningMsgStr = warningMsg; \
        auto logWarning = [&](const char* msg) { \
            OutputDebugStringA("Warning: "); OutputDebugStringA(msg); OutputDebugStringA("\n"); \
            profiler.LogWarning(msg); \
            allNotNull = false; \
        }; \
        auto logInfo = [&](const char* msg) { \
            OutputDebugStringA("Success: "); OutputDebugStringA(msg); OutputDebugStringA("\n"); \
            profiler.LogInfo(msg); \
        }; \
        if (!(CheckNull(__VA_ARGS__))) { \
            logInfo(successMsgStr); \
        } else { \
            logWarning(warningMsgStr); \
        } \
    } while (false)


// For Release Instance, used in Destructor of resources
#define SAFE_RELEASE(p) \
    if ((p) != nullptr) { \
        (p)->Release(); \
        (p) = nullptr; \
    }

#define SAFE_DELETE(p) \
    if ((p) != nullptr) { \
        delete (p); \
        (p) = nullptr; \
    }

bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage);

bool CheckExtension(std::string filePath, std::string fileExtension);



