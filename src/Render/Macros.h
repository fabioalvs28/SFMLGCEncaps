#pragma once

bool CheckHResult(HRESULT hr, const std::string& msg);
bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage);
bool CheckExtension(std::string filePath, std::string fileExtension);
bool CompareShaderMeshFlags(GCMaterial* pMaterial, GCMesh* pMesh);

template <typename Iterator, typename Container>
bool LogRemoveResource(Iterator it, const std::string& resourceName, Container& container)
{
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    if (it == std::end(container))
    {
        std::wstring wideMessage = L"Resource " + std::wstring(resourceName.begin(), resourceName.end()) + L" not found, can't remove it\n";
        OutputDebugString(wideMessage.c_str());
        profiler.LogWarning("Resource " + resourceName + " not found, can't remove it");
        return false;
    }
    else
    {
        std::wstring wideMessage = L"Resource " + std::wstring(resourceName.begin(), resourceName.end()) + L" removed successfully\n";
        OutputDebugString(wideMessage.c_str());
        profiler.LogInfo("Resource " + resourceName + " removed successfully");
        return true;
    }
}

template<class T>
bool CheckNull(T value)
{

	if (value == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}

template<class T, class ...Args>
bool CheckNull(T value, Args ... args)
{

	if (value == nullptr)
	{
		return false;
	}
	else 
	{
		return CheckNull(args...);
	}
}


template<typename... Args>
bool CheckPointersNull(const char* successMsg, const char* warningMsg, Args... args)
{
    GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();
    do {
        bool allNotNull = true;
        const char* successMsgStr = successMsg;
        const char* warningMsgStr = warningMsg;

        auto logWarning = [&](const char* msg) {
            OutputDebugStringA("Warning: ");
            OutputDebugStringA(msg);
            OutputDebugStringA("\n");
            profiler.LogWarning(msg);
            allNotNull = false;
            };

        auto logInfo = [&](const char* msg) {
            OutputDebugStringA("Success: ");
            OutputDebugStringA(msg);
            OutputDebugStringA("\n");
            profiler.LogInfo(msg);
            };

        if (!CheckNull(args...)) {
            logWarning(warningMsgStr);
            return false;
        }
        else {
            logInfo(successMsgStr);
            return true;
        }
    } while (false);
}

// Debug Only ***

// Variadic Check Ptr with 1 message Error 
#ifdef _PROFILER
#define CHECK_POINTERSNULL(successMsg, warningMsg, ...) \
    CheckPointersNull(successMsg, warningMsg, __VA_ARGS__)
#else
#define CHECK_POINTERSNULL(successMsg, warningMsg, ...) \
    true
#endif

#ifdef _PROFILER
#define CHECK_FILE(fileName, errorMessage, successMessage) \
    CheckFile(fileName, errorMessage, successMessage)
#else
#define CHECK_FILE(fileName, errorMessage, successMessage) \
    true
#endif

#ifdef _PROFILER
#define CHECK_EXTENSION(filePath, fileExtension) \
    CheckExtension(filePath, fileExtension)
#else
#define CHECK_EXTENSION(filePath, fileExtension) \
    true
#endif

#ifdef _PROFILER
#define CHECK_HRESULT(hr, msg) \
    CheckHResult(hr, msg)
#else
#define CHECK_HRESULT(hr, msg) \
    true
#endif


#ifdef _PROFILER
#define COMPARE_SHADER_MESH_FLAGS(material, mesh) \
    CompareShaderMeshFlags(material, mesh)
#else
#define COMPARE_SHADER_MESH_FLAGS(material, mesh) \
    true
#endif

// Release & Debug ***

#define LOG_REMOVE_RESOURCE(it, resourceName, container) \
    LogRemoveResource(it, resourceName, container)


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

// Vertex Input Layout Flags
#define VERTEX_POSITION  0x01 // 00000001
#define VERTEX_COLOR     0x02 // 00000010
#define VERTEX_UV        0x04 // 00000100
#define VERTEX_NORMAL    0x08 // 00001000
#define VERTEX_TANGENT   0x10 // 00010000
#define VERTEX_BINORMAL  0x20 // 00100000

// Root Parameter Flags
#define ROOT_PARAMETER_CB0                      0x01 // 00000001
#define ROOT_PARAMETER_CB1                      0x02 // 00000010
#define ROOT_PARAMETER_CB2                      0x04 // 00000100
#define ROOT_PARAMETER_CB3                      0x08 // 00001000
#define ROOT_PARAMETER_DESCRIPTOR_TABLE_SLOT1   0x10 // 00010000
#define ROOT_PARAMETER_DESCRIPTOR_TABLE_SLOT2   0x20 // 00100000


// Check if a specific flag is set
#define HAS_FLAG(flags, flag) (((flags) & (flag)) != 0)

// Set a specific flag
#define SET_FLAG(flags, flag) ((flags) |= (flag))

// Unset a specific flag
#define UNSET_FLAG(flags, flag) ((flags) &= ~(flag))

// Lights Type
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_SPOT 1
#define LIGHT_TYPE_POINT 2

// 
#ifndef ReleaseCom
#define ReleaseCom(x) { if(x){ x->Release(); x = 0; } }
#endif


// Default Flag
#define DEFAULT_ROOT_PARAMETER_FLAG 0b00111111 // All Flag