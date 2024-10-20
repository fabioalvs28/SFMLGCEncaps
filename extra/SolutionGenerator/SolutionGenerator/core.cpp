#include "core.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <objbase.h>
#include <filesystem>
#include <unordered_set>
#include <random>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
using namespace tinyxml2;
using namespace std; 
namespace fs = filesystem;

extern const char* g_contentMainConsole;
extern const char* g_contentMainWindows;
extern const char* g_contentMainLib;
extern const char* g_contentRc;
extern const char* g_contentRes;
extern const char* g_contentPch;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GCSolutionGenerator::GCSolutionGenerator()
{
	m_srcExt = "cpp;c";
	m_hExt = "h;hpp";
	m_rcExt = "hlsl;jpg;jpeg;png;wav;mp3;ico;dds;rc;obj";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GCSolutionGenerator::CommandCreate(vector<string>& args)
{
	if ( args.size()<3 )
	{
		cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
		return false;
	}

	string folder = ToFolder(args[0]);
	string solutionName = args[1];
	string projectName = args[2];
	bool flagWindows = false;
	bool flagLib = false;
	for ( int i=3 ; i<args.size() ; i++ )
	{
		if ( flagWindows==false && args[i]=="-windows" )
			flagWindows = true;
		if ( flagLib==false && args[i]=="-lib" )
			flagLib = true;
	}

	if ( fs::exists(folder)==false )
		fs::create_directories(folder);

	if ( fs::exists(folder+"config")==false )
		fs::create_directories(folder+"config");

	if ( fs::exists(folder+"ide")==false )
		fs::create_directories(folder+"ide");

	if ( fs::exists(folder+"ide\\vs")==false )
		fs::create_directories(folder+"ide\\vs");

	if ( fs::exists(folder+"bin")==false )
	{
		fs::create_directories(folder+"bin");
		ofstream tmp(folder+"bin\\readme.txt");
		tmp.close();
	}

	if ( fs::exists(folder+"res")==false )
	{
		fs::create_directories(folder+"res");
		ofstream tmp(folder+"res\\readme.txt");
		tmp.close();
	}

	if ( fs::exists(folder+"doc")==false )
	{
		fs::create_directories(folder+"doc");
		ofstream tmp(folder+"doc\\readme.txt");
		tmp.close();
	}

	if ( fs::exists(folder+"src")==false )
	{
		fs::create_directories(folder+"src");
		CreateProjectFiles(folder, projectName, flagWindows, flagLib);
	}

	CreateJsonSolution(folder, solutionName, projectName);
	CreateJsonProject(folder, projectName, flagWindows, flagLib);
	return true;
}

bool GCSolutionGenerator::CommandAdd(vector<string>& args)
{
	if ( args.size()<2 )
	{
		cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
		return false;
	}

	string folder = ToFolder(args[0]);
	string projectName = args[1];
	bool flagWindows = false;
	bool flagLib = false;
	for ( int i=2 ; i<args.size() ; i++ )
	{
		if ( flagWindows==false && args[i]=="-windows" )
			flagWindows = true;
		if ( flagLib==false && args[i]=="-lib" )
			flagLib = true;
	}

	CreateProjectFiles(folder, projectName, flagWindows, flagLib);
	CreateJsonProject(folder, projectName, flagWindows, flagLib);
	ModifyJsonSolution(folder, projectName);
	return true;
}

bool GCSolutionGenerator::CommandMake(vector<string>& args)
{
	if ( args.size()<1 )
	{
		cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
		return false;
	}

	bool isForceRemove = false;
	if ( args.size()>=2 && string(args[1])=="-clean" )
		isForceRemove = true;

	return GenerateVisualSolution(ToFolder(args[0]), isForceRemove);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCSolutionGenerator::CreateJsonSolution(string folder, string solutionName, string projectName)
{
	if ( folder=="" )
		return;

	ordered_json data =
	{
		{"solution_name", solutionName},
		{"format_version", "12.00"},
		{"version", "16"},
		{"version_full", "16.0.30319.14"},
		{"minimum_version", "10.0.40219.1"},
		{"projects", nlohmann::json::array({projectName + ".prj"})}
	};

	string path = folder + "config\\" + solutionName + ".sol";
	WriteJsonFile(path, data);
	std::cout << "Created default solution json file: " << path << endl;
}

void GCSolutionGenerator::ModifyJsonSolution(string folder, string projectNameToAdd)
{
	if ( folder=="" )
		return;

	string solutionPath = FindFirstSolFile(folder);
	if ( solutionPath=="" )
	{
		cerr << "Solution not found." << endl;
		return;
	}

	json data;
	ifstream src(solutionPath);
	src >> data;
	src.close();

	data["projects"].push_back(projectNameToAdd+".prj");
	
	ofstream trg(solutionPath);
	trg << data.dump(4);
	trg.close();

	std::cout << "Solution modified: " << solutionPath << endl;
}

void GCSolutionGenerator::CreateJsonProject(string folder, string projectName, bool windows, bool lib)
{
	if ( folder=="" )
		return;

	string type = lib ? "StaticLibrary" : "Application";
	string prepro = windows ? "_WINDOWS" : "_CONSOLE";
	string subsystem = windows ? "Windows" : "Console";

	ordered_json data =
	{
		{"name", projectName},
		{"folder", projectName + "\\"},
		{"vc_project_version", "17.0"},
		{"root_namespace", projectName},
		{"windows_target_platform_version", "10.0"},
		{"use_vcpkg", "false"},
		{"use_vcpkg_manifest", "false"},
		{"configuration", nlohmann::json::array({
			{
				{"name", "Debug|x64"},
				{"type", type},
				{"use_debug_libraries", "true"},
				{"platform_toolset", "v143"},
				{"character_set", "Unicode"},
				{"warning_level", "Level3"},
				{"sdl_check", "true"},
				{"vcpkg_configuration", "Debug"},
				{"preprocessor_definitions", "_DEBUG;"+prepro+";%(PreprocessorDefinitions)"},
				{"conformance_mode", "true"},
				{"precompiled_header", "Use"},
				{"precompiled_header_file", "pch.h"},
				{"subsystem", subsystem},
				{"language_standard", "stdcpp20"},
				{"generate_debug_information", "true"},
				{"additional_include_directories", ""},
				{"additional_library_directories", ""},
				{"additional_dependencies", "%(AdditionalDependencies)"},
				{"out_dir", "$(ProjectDir)Build\\$(Configuration)\\"},
				{"int_dir", "$(ProjectDir)Build\\$(Configuration)\\intermediate\\"}
			},
			{
				{"name", "Release|x64"},
				{"type", type},
				{"use_debug_libraries", "false"},
				{"platform_toolset", "v143"},
				{"character_set", "Unicode"},
				{"warning_level", "Level3"},
				{"vcpkg_configuration", "Release"},
				{"sdl_check", "true"},
				{"preprocessor_definitions", "NDEBUG;"+prepro+";%(PreprocessorDefinitions)"},
				{"conformance_mode", "true"},
				{"precompiled_header", "Use"},
				{"precompiled_header_file", "pch.h"},
				{"subsystem", subsystem},
				{"language_standard", "stdcpp20"},
				{"generate_debug_information", "false"},
				{"additional_include_directories", ""},
				{"additional_library_directories", ""},
				{"additional_dependencies", "%(AdditionalDependencies)"},
				{"out_dir", "$(ProjectDir)Build\\$(Configuration)\\"},
				{"int_dir", "$(ProjectDir)Build\\$(Configuration)\\intermediate\\"}
			},
		})},
	};

	string path = folder + "config\\" + projectName + ".prj";
	WriteJsonFile(path, data);
	std::cout << "Created default project json file: " << path << endl;
}

bool GCSolutionGenerator::CreateProjectFiles(string folder, string projectName, bool flagWindows, bool flagLib)
{
	if ( folder=="" )
		return false;

	string projectFolder = folder + "src\\" + projectName + "\\";
	if ( fs::exists(projectFolder) )
		return false;

	fs::create_directories(projectFolder);
	string data = "";

	// main.cpp
	if ( flagWindows )
	{
		if ( flagLib )
			data = g_contentMainLib;
		else
			data = g_contentMainWindows;
	}
	else
	{
		if ( flagLib )
			data = g_contentMainLib;
		else
			data = g_contentMainConsole;
	}
	ofstream maincpp(projectFolder+"main.cpp");
	maincpp.write(data.c_str(), data.length());
	maincpp.close();

	// main.h
	ofstream mainh(projectFolder+"main.h");
	data = "#pragma once\n\n";
	mainh.write(data.c_str(), data.length());
	mainh.close();

	// pch.h
	ofstream pch(projectFolder+"pch.h");
	if ( flagWindows )
	{
		if ( flagLib )
			data = "#pragma once\n\n#include <windows.h>\n\n";
		else
			data = "#pragma once\n\n#include <windows.h>\n\n#include \"resource.h\"\n\n";
	}
	else
		data = "#pragma once\n\n";
	data += g_contentPch;
	pch.write(data.c_str(), data.length());
	pch.close();

	// pch.cpp
	ofstream pchcpp(projectFolder+"pch.cpp");
	data = "#include \"pch.h\"\n\n";
	pchcpp.write(data.c_str(), data.length());
	pchcpp.close();

	if ( flagLib==false )
	{
		// app.rc
		ofstream rc(projectFolder+"app.rc");
		data = g_contentRc;
		rc.write(data.c_str(), data.length());
		rc.close();

		// resource.h
		ofstream res(projectFolder+"resource.h");
		data = g_contentRes;
		res.write(data.c_str(), data.length());
		res.close();

		// app.ico
		ofstream ico(projectFolder+"app.ico");
		ico.close();

		// small.ico
		ofstream icosm(projectFolder+"small.ico");
		icosm.close();
	}
	return true;
}

string GCSolutionGenerator::FindFirstSolFile(string folder)
{
	string config = folder + "config\\";
	for ( const auto& entry : fs::directory_iterator(config) )
	{
		if ( entry.path().extension()==".sol" )
			return entry.path().string();
	}
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GCSolutionGenerator::GenerateVisualSolution(string folder, bool deleteFolder)
{
	string solutionPath = FindFirstSolFile(folder);
	if ( solutionPath=="" )
	{
		cerr << "No .sol file found." << endl;
		return false;
	}

	m_folder = folder;
	m_solutionPath = solutionPath;
	m_binFolder = m_ideFolder + "bin\\";
	m_configFolder = m_folder + "config\\";
	m_docFolder = m_ideFolder + "doc\\";
	m_srcFolder = m_folder + "src\\";
	m_ideFolder = m_folder + "ide\\";
	m_vsFolder = m_ideFolder + "vs\\";

	// Get Data
	FillDataWithSolAndPrj();

	// Delete folder without deleting source files
	if ( deleteFolder )
		DeleteFolderSafe(m_ideFolder);

	// Generate
	GenerateSln();
	for ( auto& project : m_data["contents"] )
	{
		PopulateIncludeFiles(project);
		GenerateVcxproj(project);
		GenerateFilters(project);
	}

	// Opens explorer window
	ShellExecuteA(NULL, "open", fs::absolute(m_vsFolder).string().c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}

void GCSolutionGenerator::GenerateSln()
{
	string solutionName = m_data["solution_name"];
	json& projects = m_data["contents"];
	json& folders = m_data["folders"];

	string typeGuid = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}";
	//const map<string, string> typeGuid =
	//{
	//	{"project", "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"},
	//	{"folder", "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"}
	//};

	json guids = {
		{"projects", json::object()},
		{"folders", json::object()}
	};

	for(auto& project : projects)
	{
		string guid = GenerateGuid();
		string projectName = project["name"];
		project["guid"] = guid;
		guids["projects"][projectName] = guid;
	}

	for(auto& folder : folders)
	{
		string guid = GenerateGuid();
		string folderName = folder["name"];
		folder["guid"] = guid;
		guids["folders"][folderName] = guid;
	}

	// Create a file and write into it
	fs::path filePath = m_vsFolder + solutionName + ".sln";
	fs::create_directories(m_vsFolder);
	cout << "Creating solution file: " << filePath << endl;

	ofstream outputFile(filePath);
	if ( outputFile.is_open()==false )
	{
		cerr << "Failed to create file." << filePath << endl;
		exit(1);
	}

	outputFile << endl;
	outputFile << "Microsoft Visual Studio Solution File, Format Version " << m_data["format_version"].get<string>() << endl;
	outputFile << "# Visual Studio Version " << m_data["version"].get<string>() << endl;
	outputFile << "VisualStudioVersion = " << m_data["version_full"].get<string>() << endl;
	outputFile << "MinimumVisualStudioVersion = " << m_data["minimum_version"].get<string>() << endl;

	// Add the projects
	for (const auto& project : projects)
	{
		outputFile << "Project(\"" << typeGuid << "\") = " << project["name"] << ", \"" << project["folder"].get<string>() << project["name"].get<string>() << ".vcxproj" << "\", " << project["guid"] << endl;

		// Add the dependencies
		if (project.find("dependencies") != project.end())
		{
			outputFile << "\tProjectSection(ProjectDependencies) = postProject" << endl;
			for (const auto& dependency : project["dependencies"])
			{
				string guid = guids["projects"][dependency].get<string>();
				outputFile << "\t\t" << guid << " = " << guid << endl;
			}
			outputFile << "\tEndProjectSection" << endl;
		}
		outputFile << "EndProject" << endl;
	}

	// Add the global section
	outputFile << "Global" << endl;
	outputFile << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
	for (const auto& project : projects)
	{
		auto& configs = project["configuration"];
		for ( auto& config : configs )
		{
			string name = GetStr(config, "name");
			outputFile << "\t\t" << name << " = " << name << endl;
		}
	}

	outputFile << "\tEndGlobalSection" << endl;

	// Add project configurations
	outputFile << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
	for (const auto& project : projects)
	{
		auto& configs = project["configuration"];
		for ( auto& config : configs )
		{
			string name = GetStr(config, "name");
			outputFile << "\t\t" << project["guid"].get<string>() << "." << name << ".ActiveCfg = " << name << endl;
			outputFile << "\t\t" << project["guid"].get<string>() << "." << name << ".Build.0 = " << name << endl;
		}
	}
	outputFile << "\tEndGlobalSection" << endl;

	outputFile << "\tGlobalSection(SolutionProperties) = preSolution" << endl;
	outputFile << "\t\tHideSolutionNode = FALSE" << endl;
	outputFile << "\tEndGlobalSection" << endl;

	// Add the nested projects
	outputFile << "\tGlobalSection(NestedProjects) = preSolution" << endl;
	for (const auto& project : projects) {
		if (project.find("nested") != project.end()) {
			string guid = project["guid"].get<string>();
			string nested = guids["folders"][project["nested"]].get<string>();
			outputFile << "\t\t" << guid << " = " << nested << endl;
		}
	}
	outputFile << "\tEndGlobalSection" << endl;

	outputFile << "\tGlobalSection(ExtensibilityGlobals) = postSolution" << endl;
	outputFile << "\t\tSolutionGuid = " << GenerateGuid() << endl;
	outputFile << "\tEndGlobalSection" << endl;

	outputFile << "EndGlobal" << endl;

	outputFile.close();

	cout << fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
}

void GCSolutionGenerator::GenerateVcxproj(json& project)
{
	tinyxml2::XMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration();
	doc.InsertFirstChild(decl);

	XMLElement* proj = doc.NewElement("Project");
	proj->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
	proj->SetAttribute("ToolsVersion", "4.0");
	doc.InsertEndChild(proj);

	// Add configurations
	XMLElement* itemGroup = doc.NewElement("ItemGroup");
	itemGroup->SetAttribute("Label", "ProjectConfigurations");
	proj->InsertEndChild(itemGroup);

	auto& configs = project["configuration"];
	for ( auto& config : configs )
	{
		string name = GetStr(config, "name");
		size_t pos = name.find('|');
		if ( pos==string::npos )
			continue;
		string sub = name.substr(0, pos);
		XMLElement* projectConfig = doc.NewElement("ProjectConfiguration");
		projectConfig->SetAttribute("Include", name.c_str());
		itemGroup->InsertEndChild(projectConfig);

		AddTextElement(&doc, projectConfig, "Configuration", sub);
		AddTextElement(&doc, projectConfig, "Platform", "x64");
	}

	// Globals
	XMLElement* propertyGroup = doc.NewElement("PropertyGroup");
	propertyGroup->SetAttribute("Label", "Globals");
	proj->InsertEndChild(propertyGroup);
	AddTextElement(&doc, propertyGroup, "VCProjectVersion", GetStr(project, "vc_project_version"));
	AddTextElement(&doc, propertyGroup, "Keyword", "Win32Proj");
	AddTextElement(&doc, propertyGroup, "ProjectGuid", GetStr(project, "guid"));
	AddTextElement(&doc, propertyGroup, "RootNamespace", GetStr(project, "root_namespace"));
	AddTextElement(&doc, propertyGroup, "WindowsTargetPlatformVersion", GetStr(project, "windows_target_platform_version"));

	// PropertyGroup for VCPKG
	propertyGroup = doc.NewElement("PropertyGroup");
	propertyGroup->SetAttribute("Label", "Vcpkg");
	proj->InsertEndChild(propertyGroup);
	AddTextElement(&doc, propertyGroup, "VcpkgEnable", GetStr(project, "use_vcpkg"));
	AddTextElement(&doc, propertyGroup, "VcpkgEnableManifest", GetStr(project, "use_vcpkg_manifest"));
	AddTextElement(&doc, propertyGroup, "VcpkgManifestRoot", "..\\..\\..\\config\\");
	AddTextElement(&doc, propertyGroup, "VcpkgInstalledDir", ".\\vcpkg_installed\\$(VcpkgTriplet)\\");

	// Default props
	XMLElement* importGroup = doc.NewElement("Import");
	importGroup->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.Default.props");
	proj->InsertEndChild(importGroup);

	// Configurations
	for ( const auto& config : project["configuration"] )
	{
		string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
		XMLElement* propertyGroup = doc.NewElement("PropertyGroup");
		propertyGroup->SetAttribute("Condition", condition.c_str());
		propertyGroup->SetAttribute("Label", "Configuration");
		proj->InsertEndChild(propertyGroup);

		AddTextElement(&doc, propertyGroup, "ConfigurationType", GetStr(config, "type"));
		AddTextElement(&doc, propertyGroup, "UseDebugLibraries", GetStr(config, "use_debug_libraries"));
		AddTextElement(&doc, propertyGroup, "PlatformToolset", GetStr(config, "platform_toolset"));
		AddTextElement(&doc, propertyGroup, "WholeProgramOptimization", GetStr(config, "whole_program_optimization"));
		AddTextElement(&doc, propertyGroup, "CharacterSet", GetStr(config, "character_set"));
	
		// Vcpkg Specific configuration
		propertyGroup = doc.NewElement("PropertyGroup");
		propertyGroup->SetAttribute("Condition", condition.c_str());
		propertyGroup->SetAttribute("Label", "Vcpkg");
		proj->InsertEndChild(propertyGroup);
		AddTextElement(&doc, propertyGroup, "VcpkgConfiguration", GetStr(config, "vcpkg_configuration"));
	}

	// Cpp props
	XMLElement* element = doc.NewElement("Import");
	element->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.props");
	proj->InsertEndChild(element);

	element = doc.NewElement("ImportGroup");
	element->SetAttribute("Label", "ExtensionSettings");
	proj->InsertEndChild(element);

	element = doc.NewElement("ImportGroup");
	element->SetAttribute("Label", "Shared");
	proj->InsertEndChild(element);

	// Property sheets
	for ( const auto& config : project["configuration"] )
	{
		string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
		XMLElement* importGroup = doc.NewElement("ImportGroup");
		importGroup->SetAttribute("Label", "PropertySheets");
		importGroup->SetAttribute("Condition", condition.c_str());
		proj->InsertEndChild(importGroup);

		XMLElement* element = doc.NewElement("Import");
		element->SetAttribute("Project", "$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props");
		element->SetAttribute("Condition", "exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')");
		element->SetAttribute("Label", "LocalAppDataPlatform");
		importGroup->InsertEndChild(element);
	}

	// Output Directory
	for ( const auto& config : project["configuration"] )
	{
		string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
		XMLElement* propertyGroup = doc.NewElement("PropertyGroup");
		propertyGroup->SetAttribute("Condition", condition.c_str());
		proj->InsertEndChild(propertyGroup);

		AddTextElement(&doc, propertyGroup, "OutDir", GetStr(config, "out_dir"));
		AddTextElement(&doc, propertyGroup, "IntDir", GetStr(config, "int_dir"));
	}

	// Item Definition Groups
	for ( const auto& config : project["configuration"] )
	{
		string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
		XMLElement* itemDefGroup = doc.NewElement("ItemDefinitionGroup");
		itemDefGroup->SetAttribute("Condition", condition.c_str());
		proj->InsertEndChild(itemDefGroup);

		XMLElement* clCompile = doc.NewElement("ClCompile");
		itemDefGroup->InsertEndChild(clCompile);

		AddTextElement(&doc, clCompile, "WarningLevel", GetStr(config, "warning_level"));
		AddTextElement(&doc, clCompile, "FunctionLevelLinking", GetStr(config, "function_level_linking"));
		AddTextElement(&doc, clCompile, "IntrinsicFunctions", GetStr(config, "intrinsic_functions"));
		AddTextElement(&doc, clCompile, "SDLCheck", GetStr(config, "sdl_check"));
		AddTextElement(&doc, clCompile, "PreprocessorDefinitions", GetStr(config, "preprocessor_definitions"));
		AddTextElement(&doc, clCompile, "ConformanceMode", GetStr(config, "conformance_mode"));
		AddTextElement(&doc, clCompile, "PrecompiledHeader", GetStr(config, "precompiled_header"));
		AddTextElement(&doc, clCompile, "PrecompiledHeaderFile", GetStr(config, "precompiled_header_file"));
		AddTextElement(&doc, clCompile, "LanguageStandard", GetStr(config, "language_standard"));
		AddTextElement(&doc, clCompile, "AdditionalIncludeDirectories", GetStr(config, "additional_include_directories"));

		// Link
		XMLElement* link = doc.NewElement("Link");
		itemDefGroup->InsertEndChild(link);

		AddTextElement(&doc, link, "SubSystem", GetStr(config, "subsystem"));
		AddTextElement(&doc, link, "EnableCOMDATFolding", GetStr(config, "enable_comdat_folding"));
		AddTextElement(&doc, link, "OptimizeReferences", GetStr(config, "optimize_references"));
		AddTextElement(&doc, link, "GenerateDebugInformation", GetStr(config, "generate_debug_information"));
		AddTextElement(&doc, link, "AdditionalLibraryDirectories", GetStr(config, "additional_library_directories"));
		AddTextElement(&doc, link, "AdditionalDependencies", GetStr(config, "additional_dependencies"));
	}

	// Item Groups for source files
	XMLElement* itemGroupCompile = doc.NewElement("ItemGroup");
	proj->InsertEndChild(itemGroupCompile);
	for ( const auto& source : project["source_files"] )
	{
		string str = source.get<string>();
		XMLElement* clCompile = doc.NewElement("ClCompile");
		clCompile->SetAttribute("Include", str.c_str());
		itemGroupCompile->InsertEndChild(clCompile);

		// PCH
		if ( str.ends_with("pch.cpp") )
		{
			for ( const auto& config : project["configuration"] )
			{
				string configName = GetStr(config, "name");			
				XMLElement* precompiledHeader = doc.NewElement("PrecompiledHeader");
				string cond = "'$(Configuration)|$(Platform)'=='" + configName + "'";
				precompiledHeader->SetAttribute("Condition", cond.c_str());
				precompiledHeader->SetText("Create");
				clCompile->InsertEndChild(precompiledHeader);
			}
		}
	}

	// Item Groups for header files
	XMLElement* itemGroupInclude = doc.NewElement("ItemGroup");
	proj->InsertEndChild(itemGroupInclude);
	for ( const auto& header : project["header_files"] )
	{
		string str = header.get<string>();
		XMLElement* clInclude = doc.NewElement("ClInclude");
		clInclude->SetAttribute("Include", str.c_str());
		itemGroupInclude->InsertEndChild(clInclude);
	}

	// Item Groups for resources files
	XMLElement* itemGroupNone = doc.NewElement("ItemGroup");
	proj->InsertEndChild(itemGroupNone);
	for ( const auto& file : project["resource_files"] )
	{
		string str = file.get<string>();
		XMLElement* none = doc.NewElement("None");
		none->SetAttribute("Include", str.c_str());
		itemGroupNone->InsertEndChild(none);
	}

	// Cpp targets and ExtensionTargets
	XMLElement* elem = doc.NewElement("Import");
	elem->SetAttribute("Project", "$(VCTargetsPath)\\Microsoft.Cpp.targets");
	proj->InsertEndChild(elem);

	elem = doc.NewElement("ImportGroup");
	elem->SetAttribute("Label", "ExtensionTargets");
	proj->InsertEndChild(elem);

	// Save the vcxproj file
	string folderPath = m_vsFolder + project["folder"].get<string>();
	string filePath = folderPath + project["name"].get<string>() + ".vcxproj";
	fs::create_directories(folderPath);
	XMLError eResult = doc.SaveFile(filePath.c_str());
	if ( eResult!=XML_SUCCESS )
	{
		cerr << RED << "Error saving file: " << eResult << RESET << endl;
		//return;
	}

	std::cout << fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
}

void GCSolutionGenerator::GenerateFilters(json& project)
{
	tinyxml2::XMLDocument doc;

	XMLDeclaration* decl = doc.NewDeclaration();
	doc.InsertFirstChild(decl);

	XMLElement* proj = doc.NewElement("Project");
	proj->SetAttribute("xmlns", "http://schemas.microsoft.com/developer/msbuild/2003");
	proj->SetAttribute("ToolsVersion", "4.0");
	doc.InsertEndChild(proj);

	XMLElement* itemGroup = doc.NewElement("ItemGroup");
	proj->InsertEndChild(itemGroup);

	map<string, string> filters = {
		{"header", m_hExt},
		{"resource", m_rcExt},
		{"source", m_srcExt}
		};

	for ( const auto& filter : filters )
	{
		string key = filter.first + "_files";
		for ( auto path : project[key] )
		{
			string name = filter.first + GetFilter(ToRelativeProjectPath(path));
			if ( name!="" && filters.find(name)==filters.end() )
				filters.insert(make_pair(name, filter.second));
		}
	}

	// Create filters
	for ( const auto& filter : filters )
	{
		XMLElement* filterElement = doc.NewElement("Filter");
		filterElement->SetAttribute("Include", filter.first.c_str());
		itemGroup->InsertEndChild(filterElement);

		XMLElement* elem = doc.NewElement("UniqueIdentifier");
		elem->SetText(GenerateGuid().c_str());
		filterElement->InsertEndChild(elem);

		elem = doc.NewElement("Extensions");
		elem->SetText(filter.second.c_str());
		filterElement->InsertEndChild(elem);
	}

	// Add files to their respective filters
	itemGroup = doc.NewElement("ItemGroup");
	proj->InsertEndChild(itemGroup);
	AddFilesToItemGroup(&doc, itemGroup, "ClCompile", project["source_files"], "source");
	AddFilesToItemGroup(&doc, itemGroup, "ClInclude", project["header_files"], "header");
	AddFilesToItemGroup(&doc, itemGroup, "None", project["resource_files"], "resource");

	// Save the vcxproj file
	string folderPath = m_vsFolder + project["folder"].get<string>();
	string filePath = folderPath + project["name"].get<string>() + ".vcxproj.filters";
	fs::create_directories(folderPath);
	XMLError eResult = doc.SaveFile(filePath.c_str());
	if ( eResult!=XML_SUCCESS )
	{
		cerr << RED << "Error saving file: " << eResult << RESET << endl;
		//return;
	}

	cout << fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCSolutionGenerator::FillDataWithSolAndPrj()
{
	json data = json::object();

	ifstream inputPath(m_solutionPath);
	cout << "Reading data from " << m_solutionPath << endl;
	if ( inputPath.is_open()==false )
	{
		cerr << "Could not open the file " << m_solutionPath << endl;
		return;
	}
		
	data = ReadJsonFile(m_solutionPath);
	inputPath.close();

	data["folders"] = json::array();
	data["contents"] = json::array();
	for ( auto project : data["projects"] )
	{
		string name = project.get<string>();
		json file_data = ReadJsonFile(fs::path(m_solutionPath).parent_path().string() + "\\" + name);
		data["contents"].push_back(file_data);

		json folder = json::object();
		folder["name"] = name.substr(0, name.length()-4);
		data["folders"].push_back(folder);		
	} 

	m_data = data;
}

void GCSolutionGenerator::PopulateIncludeFiles(json& project)
{
	unordered_set<string> srcExt = SplitString(m_srcExt, ';');
	unordered_set<string> hExt = SplitString(m_hExt, ';');
	unordered_set<string> rcExt = SplitString(m_rcExt, ';');

	array<string, 4> filters = { "source_files", "header_files", "resource_files", "other_files" };
	for ( const string& filter : filters )
	{
		if ( project.find(filter)==project.end() )
			project[filter] = json::array();
	}

	string path = m_srcFolder + project["folder"].get<string>();
	if ( fs::exists(path) )
	{
		for ( const auto& entry : fs::recursive_directory_iterator(path) )
		{
			if ( fs::is_regular_file(entry)==false )
				continue;

			//cout << "Fichier : " << entry.path().string() << endl;
			string ext = entry.path().extension().string().substr(1);
			string relPath = RelativePath(m_vsFolder+project["folder"].get<string>(), entry.path().string());
			bool nowhere = true;

			if ( find(srcExt.begin(), srcExt.end(), ext)!=srcExt.end() )
			{
				project["source_files"].push_back(relPath);
				nowhere = false;
			}
			if ( find(hExt.begin(), hExt.end(), ext)!=hExt.end() )
			{
				project["header_files"].push_back(relPath);
				nowhere = false;
			}
			if ( find(rcExt.begin(), rcExt.end(), ext)!=rcExt.end() )
			{
				project["resource_files"].push_back(relPath);
				nowhere = false;
			}
			if ( nowhere )
				project["other_files"].push_back(relPath);
		}
	}
	else
	{
		fs::create_directories(path);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCSolutionGenerator::WriteJsonFile(const string& fileName, const ordered_json& data)
{
	ofstream outputFile(fileName);
	if ( outputFile.is_open() )
	{
		outputFile << data.dump(4);
		outputFile.close();
	}
	else
	{
		cerr << "Unable to open file for writing: " << fileName << endl;
	}
}

json GCSolutionGenerator::ReadJsonFile(string fileName)
{
	ifstream inputFile(fileName);

	if ( inputFile.is_open()==false )
	{
		cerr << RED << "Could not open the file " << fileName << RESET << endl;
		json data;
		return data;
	}

	json data;
	inputFile >> data;
	inputFile.close();
	return data;
}

string GCSolutionGenerator::GetStr(json obj, string key)
{
	if ( obj.contains(key) )
		return obj[key].get<string>();
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCSolutionGenerator::AddTextElement(tinyxml2::XMLDocument* doc, XMLElement* parent, const string name, const string value)
{
	if ( value.empty() )
		return;
	XMLElement* element = doc->NewElement(name.c_str());
	element->SetText(value.c_str());
	parent->InsertEndChild(element);
}

void GCSolutionGenerator::AddFilesToItemGroup(tinyxml2::XMLDocument* doc, XMLElement* itemGroup, const string tag, json files, const string root)
{
	for ( const auto& file : files )
	{
		XMLElement* element = doc->NewElement(tag.c_str());
		string path = file.get<string>();
		element->SetAttribute("Include", path.c_str());
		itemGroup->InsertEndChild(element);

		XMLElement* filterElement = doc->NewElement("Filter");
		string name = root + GetFilter(ToRelativeProjectPath(path));
		filterElement->SetText(name.c_str());
		element->InsertEndChild(filterElement);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unordered_set<string> GCSolutionGenerator::SplitString(string str, char delimiter)
{
	unordered_set<string> result;
	istringstream iss(str);
	string token;
	while ( getline(iss, token, delimiter) )
		result.insert(token);
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string GCSolutionGenerator::ToFolder(string folder)
{
	if ( folder=="" )
		return folder;

	if ( folder[folder.length()-1]=='\\' || folder[folder.length()-1]=='/' )
		return folder;

	return folder + "\\";
}

string GCSolutionGenerator::GetFolder(string file)
{
	size_t index = file.rfind("\\");
	if ( index==string::npos )
		return "";
	return file.substr(0, index+1);
}

string GCSolutionGenerator::GetFilter(string file)
{
	size_t index = file.rfind("\\");
	if ( index==string::npos )
		return "";
	return "\\" + file.substr(0, index);
}

string GCSolutionGenerator::ToRelativeProjectPath(string relativeSolutionPath)
{
	string path = relativeSolutionPath.substr(13);
	size_t index = path.find("\\");
	if ( index==string::npos )
		return "";
	path = path.substr(index+1);
	index = path.find("\"");
	if ( index==string::npos )
		return path;
	return path.substr(0, index);
}

string GCSolutionGenerator::RelativePath(const string& from, const string& to)
{
	// Résoudre les chemins absolus pour éviter les problèmes de points de départ
	fs::path absFrom = fs::absolute(from);
	fs::path absTo = fs::absolute(to);

	if ( fs::exists(absFrom)==false )
		fs::create_directories(absFrom);
	if ( fs::exists(absTo)==false )
		fs::create_directories(absTo);

	// Convertir en chemins canoniques pour normaliser les chemins et gérer les points de départ différents
	absFrom = fs::canonical(absFrom); 
	absTo = fs::canonical(absTo);

	// Obtenir les itérateurs pour les chemins
	auto itFrom = absFrom.begin();
	auto itTo = absTo.begin();

	// Trouver le point de divergence
	while ( itFrom != absFrom.end() && itTo != absTo.end() && *itFrom == *itTo )
	{
		++itFrom;
		++itTo;
	}

	// Construire le chemin relatif à partir du point de divergence
	fs::path result;
	for ( ; itFrom != absFrom.end(); ++itFrom )
	{
		if ( (*itFrom) != "." )
			result /= "..";
	}
	for ( ; itTo != absTo.end() ; ++itTo )
		result /= *itTo;

	return result.string();
}

bool GCSolutionGenerator::DeleteFolderSafe(string path)
{
	if ( fs::exists(path) )
	{
		unordered_set<string> srcExt = SplitString(m_srcExt, ';');
		unordered_set<string> hExt = SplitString(m_hExt, ';');
		unordered_set<string> rcExt = SplitString(m_rcExt, ';');

		array<string, 3> excludeDirs = {".vs", "Build", "vcpkg_installed"};
		fs::path fsPath;
		if (path.back() == '/') 
			fsPath = path.substr(0, path.size() - 1);
		else
			fsPath = path;

		fsPath = fsPath.filename();

		bool canDelete = true;
		if ( find(excludeDirs.begin(), excludeDirs.end(), fsPath) == excludeDirs.end() )
		{
			for ( const auto& entry : fs::directory_iterator(path) )
			{
				if ( fs::is_directory(entry) )
				{
					//cout << "Folder: " << entry.path().string() << endl;
					bool tmpCanDelete = DeleteFolderSafe(entry.path().string());
					if (tmpCanDelete == false)
						canDelete = false;
				}
				if (fs::is_regular_file(entry))
				{
					//cout << "File: " << entry.path().string() << endl;
					string ext = entry.path().extension().string();
					if ( !ext.empty() )
					{
						ext = ext.substr(1);

						auto itSrc = find(srcExt.begin(), srcExt.end(), ext);
						auto itH = find(hExt.begin(), hExt.end(), ext);
						auto itRc = find(rcExt.begin(), rcExt.end(), ext);
						bool filePresent = itSrc != srcExt.end() || itH != hExt.end() || itRc != rcExt.end();

						if (filePresent)
						{
							cout << YELLOW << "WARNING: the source file " << entry.path() << " is still present and won't be deleted." << " Do you want to move it to the src folder ?" << RESET << endl;
							char yesNo = 0;
							while (yesNo != 'y' && yesNo != 'n')
							{
								cout << "y/n : ";
								cin >> yesNo;
								yesNo = tolower(yesNo);
							}
							if (yesNo == 'y')
							{
								bool hasMoved = MoveFileToSource(entry.path().string());
								if (hasMoved == false)
									canDelete = false;
							}
							else
							{
								canDelete = false;
							}
						}
					}
				}
			}
		}

		if ( canDelete )
		{
			//cout << "Delete: " << path << endl;
			error_code ec;
			fs::remove_all(path, ec);
			if ( ec )
				cerr << RED << "Failed to remove directory: " << ec.message() << RESET << endl;
			return true;
		}
	}
	return false;
}

bool GCSolutionGenerator::MoveFileToSource(string path) 
{
	string str = RelativePath(m_vsFolder, path);
	fs::path source = fs::absolute(path);
	fs::path destination = fs::absolute(m_srcFolder+str);

	try
	{
		if ( fs::exists(destination.parent_path())==false )
			fs::create_directories(destination.parent_path());
		rename(source, destination);
		return true;
	}
	catch (const fs::filesystem_error& e)
	{
		cerr << RED << "Error while moving file: " << e.what() << RESET << endl;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string GCSolutionGenerator::GenerateGuid()
{
	GUID guid;
	HRESULT hCreateGuid = CoCreateGuid(&guid); 
	
	if ( hCreateGuid!=S_OK )
	{
		cerr << RED << "Erreur lors de la generation du GUID" << RESET << endl;
		return "";
	}

	// Convertir le GUID en une chaine de caracteres
	wchar_t wszGuid[40] = { 0 };
	StringFromGUID2(guid, wszGuid, 40);

	// Convertir la chaine wide char en string
	char szGuid[40] = { 0 };
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, szGuid, sizeof(szGuid), wszGuid, _TRUNCATE);

	return string(szGuid);
}
