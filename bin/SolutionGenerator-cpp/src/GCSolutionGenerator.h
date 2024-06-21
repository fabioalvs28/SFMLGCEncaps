#pragma once
#include <unordered_set>
#include <string>
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
	// Main function to generate the whole solution
	static void GenerateSolution(std::unordered_set<std::string> args, bool deleteFolder);

private:
	GCSolutionGenerator() = delete;

	// Paths
	static const std::string s_root;
	static const std::string s_jsonPath;
	static const std::string s_idePath;
	static const std::string s_vsPath;
	static const std::string s_srcPath;
	// Extensions
	static const std::string s_srcExt;
	static const std::string s_hExt;
	static const std::string s_rcExt;
	// Data
	static nlohmann::json m_data;
	static std::unordered_set<std::string> m_args;

	// File Generation
	static void GenerateSln();
	static void GenerateVcxproj(nlohmann::json& project);
	static void GenerateFilters(nlohmann::json& project);
	// Getting data
	static void FillData();
	static void PopulateIncludeFiles(nlohmann::json& project);
	// Utils
	static nlohmann::json ReadJsonFile(std::string fileName);
	static std::string GenerateGuid();
	static std::unordered_set<std::string> SplitString(std::string str, char delimiter);
	static std::string RelativePath(const std::string& from, const std::string& to);
	static void AddTextElement(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* parent, const std::string name, const std::string value);
	static void AddFilesToItemGroup(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* itemGroup, const std::string tag, nlohmann::json files, const std::string filterName);
	static std::string GetStr(nlohmann::json obj, std::string key);
	static bool DeleteFolderSafe(std::string path);
	static bool MoveFileToSource(std::string path);
};