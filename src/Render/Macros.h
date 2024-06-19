#pragma once

template<class T>
bool CheckPointer(T value, std::string errorMessage, std::string successMessage)
{
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::wstring wideErrorMessage(errorMessage.begin(), errorMessage.end());
	std::wstring wideSuccessMessage(successMessage.begin(), successMessage.end());

	if (value == nullptr)
	{
		OutputDebugString(wideErrorMessage.c_str());
		profiler.LogWarning(errorMessage);
		return false;
	}
	else
	{
		OutputDebugString(wideSuccessMessage.c_str());
		profiler.LogInfo(successMessage);
		return true;
	}
}

template<class T, class ...Args>
bool CheckPointer(T value, Args ... args, std::string errorMessage, std::string successMessage)
{
	//check if one or multiple pointers are null
	GCGraphicsProfiler& profiler = GCGraphicsProfiler::GetInstance();

	std::wstring wideErrorMessage(errorMessage.begin(), errorMessage.end());
	std::wstring wideSuccessMessage(successMessage.begin(), successMessage.end());

	if (value == nullptr)
	{
		OutputDebugString(wideErrorMessage.c_str());
		profiler.LogWarning(errorMessage);
		return false;
	}
	else 
	{
		OutputDebugString(wideSuccessMessage.c_str());
		profiler.LogInfo(successMessage);
		return CheckNull(args..., errorMessage, successMessage);
	}
}

bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage)
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

bool CheckExtension(std::string filePath, std::string fileExtension)
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
	}

	else 
	{
		OutputDebugString((L"The file at filepath: " + wideFilePath + L"has the right type: " + wideFileExtension + L"\n").c_str());
		profiler.LogInfo("The file at filepath: " + filePath + "has the right type:" + fileExtension);
	}

}
