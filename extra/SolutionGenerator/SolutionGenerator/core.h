#pragma once
#include <unordered_set>
#include <string>
#include <vector>
#include "json.hpp"
#include "tinyxml2.h"

// ANSI color codes for console
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

class GCSolutionGenerator
{
public:
	GCSolutionGenerator();

	// Commands
	bool CommandCreate(std::vector<std::string>& args);
	bool CommandAdd(std::vector<std::string>& args);
	bool CommandMake(std::vector<std::string>& args);

private:

	// Create/Add
	void CreateJsonSolution(std::string folder, std::string solutionName, std::string projectName);
	void ModifyJsonSolution(std::string folder, std::string projectNameToAdd);
	void CreateJsonProject(std::string folder, std::string projectName, bool windows, bool lib);
	bool CreateProjectFiles(std::string folder, std::string projectName, bool flagWindows, bool flagLib);
	std::string FindFirstSolFile(std::string folder);

	// Make
	bool GenerateSolutionSolAndPrj(std::string folder, bool deleteFolder);
	void GenerateSln();
	void GenerateVcxproj(nlohmann::json& project);
	void GenerateFilters(nlohmann::json& project);





	void WriteJsonFile(const std::string& fileName, const nlohmann::ordered_json& data);

	void FillData();
	void FindAndReadSolFiles(const std::string& path, nlohmann::json& data);
	void FillDataWithSolAndPrj();
	void PopulateIncludeFiles(nlohmann::json& project);

	// Utils
	nlohmann::json ReadJsonFile(std::string fileName);
	std::string GenerateGuid();
	std::unordered_set<std::string> SplitString(std::string str, char delimiter);
	std::string RelativePath(const std::string& from, const std::string& to);
	void AddTextElement(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* parent, const std::string name, const std::string value);
	void AddFilesToItemGroup(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* itemGroup, const std::string tag, nlohmann::json files, const std::string filterName);
	std::string GetStr(nlohmann::json obj, std::string key);
	bool DeleteFolderSafe(std::string path);
	bool MoveFileToSource(std::string path);
	std::string ToFolder(std::string folder);

private:
	// Paths
	std::string m_folder;
	std::string m_configFolder;
	std::string m_solutionPath;
	std::string m_ideFolder;
	std::string m_vsFolder;
	std::string m_srcFolder;

	std::string m_projectName;
	std::string m_solutionName;

	// Extensions
	std::string m_srcExt;
	std::string m_hExt;
	std::string m_rcExt;

	// Data
	nlohmann::json m_data;
};


