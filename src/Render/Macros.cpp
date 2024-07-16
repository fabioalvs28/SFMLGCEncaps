#include "pch.h"

bool CheckFile(std::string fileName, std::string errorMessage, std::string successMessage)
{
	//checks if the filepath exists 
	// returns true if it exists 
	// false if it doesn't
	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

	std::wstring wideFileName(fileName.begin(), fileName.end());

	std::wstring wideErrorMessage(errorMessage.begin(), errorMessage.end());
	std::wstring wideSuccessMessage(successMessage.begin(), successMessage.end());

	if (_waccess(wideFileName.c_str(), 4) == -1)
	{
		OutputDebugString(wideErrorMessage.c_str());
		profiler.LogWarning("Model file not found: " + fileName);
		return false;
	}
	else
	{
		OutputDebugString(wideSuccessMessage.c_str());
		profiler.LogInfo("Model file:" + fileName + " loaded successfully");
		return true;
	}
}


bool CheckExtension(std::string filePath, std::string fileExtension)
{
	//checks if the filpeath has the right extension 
	// returns true if it is
	//  false if it isn't
	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

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

bool CheckHResult(HRESULT hr, const std::string& msg)
{
	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

	if (FAILED(hr)) {
		// Convert std::string to std::wstring
		std::wstring wMsg(msg.begin(), msg.end());
		OutputDebugString((L"Error: " + wMsg).c_str());
		profiler.LogWarning(msg);

		// Check if the HRESULT is a Windows error
		if (FACILITY_WINDOWS == HRESULT_FACILITY(hr)) {
			hr = HRESULT_CODE(hr);
		}

		// Retrieve the system error message for the given HRESULT
		TCHAR* szErrMsg;
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrMsg, 0, NULL) != 0) {
			std::wcout << szErrMsg;
			LocalFree(szErrMsg);
		}
		else {
			std::wcout << L"Could not find a description for error " << hr << L'\n';
		}

		__debugbreak();
		return false;
	}

	return true;
}

bool CompareShaderMeshFlags(GCMaterial* pMaterial, GCMesh* pMesh)
{
	GCGraphicsLogger& profiler = GCGraphicsLogger::GetInstance();

	if (pMaterial->GetShader()->GetFlagEnabledBits() == pMesh->GetFlagEnabledBits()) {
		/*profiler.LogInfo("Shader flag identique au Mesh flag");*/
		return true;
	}
	else {
		profiler.LogWarning("Shader flag pas identique au Mesh flag");
		return false;
	}
	return true;
}