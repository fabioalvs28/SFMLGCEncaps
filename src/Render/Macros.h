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

// Define flags
#define HAS_POSITION  0x01 // 00000001
#define HAS_COLOR     0x02 // 00000010
#define HAS_UV        0x04 // 00000100
#define HAS_NORMAL    0x08 // 00001000
#define HAS_TANGENT   0x10 // 00010000
#define HAS_BINORMAL  0x20 // 00100000

// Check if a specific flag is set
#define HAS_FLAG(flags, flag) (((flags) & (flag)) != 0)

// Set a specific flag
#define SET_FLAG(flags, flag) ((flags) |= (flag))

// Unset a specific flag
#define UNSET_FLAG(flags, flag) ((flags) &= ~(flag))

// Emplacement Root Parameter Index
#define CBV_SLOT_CB0 0
#define CBV_SLOT_CB1 1
#define DESCRIPTOR_TABLE_SLOT_TEXTURE 2

bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage);

bool CheckExtension(std::string filePath, std::string fileExtension);



