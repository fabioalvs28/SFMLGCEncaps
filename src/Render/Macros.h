#pragma once

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


inline bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage)
{
	//checks if the filepath exists 
	// returns true if it exists 
	// false if it doesn't
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::wstring wideFileName(fileName.begin(), fileName.end());

	std::wstring wideErrorMessage(errorMessage.begin(), errorMessage.end());
	std::wstring wideSuccessMessage(successMessage.begin(), successMessage.end());

	if (_waccess(wideFileName.c_str(), 0) == -1)
	{
		OutputDebugString(wideErrorMessage.c_str());
		profiler.LogWarning("Model file not found: " + fileName);
		
		return true;
	}
	else
	{
		OutputDebugString(wideSuccessMessage.c_str());
		profiler.LogInfo("Model file:" + fileName + " loaded successfully");
		
		return false;
	}
}

inline bool CheckExtension(std::string filePath, std::string fileExtension)
{
	//checks if the filpeath has the right extension 
	// returns true if it is
	//  false if it isn't
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::string extensionCheck;
	for (int i = 0; i < 3; i++)
	{
		extensionCheck.push_back(filePath[filePath.size() - 3 + i]);
	}

	std::wstring wideFilePath(filePath.begin(), filePath.end());
	std::wstring wideFileExtension(fileExtension.begin(), fileExtension.end());

	if (extensionCheck != fileExtension)
	{
		OutputDebugString((L"The file at filepath: " + wideFilePath + L"is not of type" + wideFileExtension + L"\n").c_str());
		profiler.LogWarning("The file at filepath: " + filePath + "is not of type" + fileExtension);
		return false;
	}

	else 
	{
		OutputDebugString((L"The file at filepath: " + wideFilePath + L"has the right type: " + wideFileExtension + L"\n").c_str());
		profiler.LogInfo("The file at filepath: " + filePath + "has the right type:" + fileExtension);
		return true;
	}

}
