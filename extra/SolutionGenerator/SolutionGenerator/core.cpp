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

// Path
string GCSolutionGenerator::s_root;
string GCSolutionGenerator::s_jsonPath;
string GCSolutionGenerator::s_srcPath;
string GCSolutionGenerator::s_idePath;
string GCSolutionGenerator::s_vsPath;
string GCSolutionGenerator::s_projectName;
string GCSolutionGenerator::s_solutionName;
string GCSolutionGenerator::s_configPath;
string GCSolutionGenerator::s_solFolder;

// Extensions
const string GCSolutionGenerator::s_srcExt = "cpp;c";
const string GCSolutionGenerator::s_hExt = "h;hpp";
const string GCSolutionGenerator::s_rcExt = "hlsl;jpg;jpeg;png;wav;mp3;ico;dds;rc;obj";

// Data
json GCSolutionGenerator::m_data;
std::unordered_map<std::string, std::string> GCSolutionGenerator::m_args;

void GCSolutionGenerator::GenerateNewSolution(std::unordered_map<std::string, std::string> args)
{
    m_args = args;

    s_jsonPath = m_args["parent_path"];
    s_projectName = m_args["project_name"];
    s_solutionName = m_args["solution_name"];

    s_srcPath = s_jsonPath + "/src";
    s_idePath = s_jsonPath + "/ide";
    s_vsPath = s_idePath + "/vs/";
}

void GCSolutionGenerator::GenerateSolutionSolAndPrj(std::unordered_map<std::string, std::string> args, bool deleteFolder)
{
    m_args = args;

    s_jsonPath = args["root_folder"];
    s_srcPath = s_jsonPath + "/src";
    s_idePath = s_jsonPath + "/ide";
    s_vsPath = s_idePath + "/vs/";
    s_configPath = args["config_folder"];
    s_solFolder = args["sol_folder"];

    // Get Data
    FillDataWithSolAndPrj();

    // Delete folder without deleting source files
    if (deleteFolder)
        DeleteFolderSafe(s_idePath);

    // Generate
    GenerateSln();
    for (auto& project : m_data["contents"])
	{
        PopulateIncludeFiles(project);
        GenerateVcxproj(project);
        GenerateFilters(project);
    }

    // Opens explorer window
    ShellExecuteA(NULL, "open", fs::absolute(s_vsPath).string().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void GCSolutionGenerator::CreateJSONSolution(const string& fileName, string projectName, string solutionName)
{
    ordered_json data =
    {
		{"solution_name", solutionName},
		{"format_version", "12.00"},
		{"version", "16"},
		{"version_full", "16.0.30319.14"},
		{"minimum_version", "10.0.40219.1"},
        {"projects", nlohmann::json::array({projectName + ".prj"})}
	};
    WriteJsonFile(fileName, data);
}

void GCSolutionGenerator::CreateJSONProject(const string& fileName, string projectName, bool windows)
{
	string prepro = windows ? "_WINDOWS" : "_CONSOLE";
	string subsystem = windows ? "Windows" : "Console";

    ordered_json data =
    {
        {"name", projectName},
        {"folder", projectName + "/"},
        {"vc_project_version", "17.0"},
        {"root_namespace", projectName},
        {"windows_target_platform_version", "10.0"},
        {"use_vcpkg", "false"},
        {"use_vcpkg_manifest", "false"},
        {"configuration", nlohmann::json::array({
            {
                {"name", "Debug|x64"},
                {"type", "Application"},
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
                {"subsystem", subsystem.c_str()},
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
                {"type", "Application"},
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
                {"subsystem", subsystem.c_str()},
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
    WriteJsonFile(fileName, data);
}

void GCSolutionGenerator::EnsureJsonFileExists(const string& solutionFileName, const string& projectFileName, const string& projectName, const string& solutionName, bool windows)
{
    if (!fs::exists(solutionFileName))
    {
        CreateJSONSolution(solutionFileName, projectName, solutionName);
        cout << "Created default solution json file: " << solutionFileName << endl;
    }

    if (!fs::exists(projectFileName))
    {
        CreateJSONProject(projectFileName, projectName, windows);
        cout << "Created default project json file: " << projectFileName << endl;
    }
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
    fs::path filePath = s_vsPath + solutionName + ".sln";
    fs::create_directories(s_vsPath);
    std::cout << "Creating solution file: " << filePath << std::endl;

    ofstream outputFile(filePath);
    if (outputFile.is_open()==false)
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
        //cout << project.dump(1) << endl;
        //outputFile << "Project(\"" << typeGuid.at("project") << "\") = " << project["name"] << ", \"" << project["folder"].get<string>() << project["name"].get<string>() << ".vcxproj" << "\", " << project["guid"] << endl;
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

    // Add the folders
    //for (const auto& folder : folders)
	//{
    //    outputFile << "Project(\"" << typeGuid.at("folder") << "\") = " << folder["name"] << ", " << folder["name"] << ", " << folder["guid"] << endl;
    //    outputFile << "EndProject" << endl;
    //}

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
    //for (const auto& folder : folders) {
    //    if (folder.find("nested") != folder.end()) {
    //        string guid = folder["guid"].get<string>();
    //        string nested = guids["folders"][folder["nested"]].get<string>();
    //        outputFile << "\t\t" << guid << " = " << nested << endl;
    //    }
    //}
    outputFile << "\tEndGlobalSection" << endl;

    outputFile << "\tGlobalSection(ExtensibilityGlobals) = postSolution" << endl;
    outputFile << "\t\tSolutionGuid = " << GenerateGuid() << endl;
    outputFile << "\tEndGlobalSection" << endl;

    outputFile << "EndGlobal" << endl;

    outputFile.close();

    std::cout << fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
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
    for (const auto& config : project["configuration"]) {
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
    for (const auto& config : project["configuration"]) {
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

    // User Macros
    //propertyGroup = doc.NewElement("PropertyGroup");
    //propertyGroup->SetAttribute("Label", "UserMacros");
    //importGroup->InsertEndChild(propertyGroup);

    // Output Directory
    for (const auto& config : project["configuration"])
	{
        string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
        XMLElement* propertyGroup = doc.NewElement("PropertyGroup");
        propertyGroup->SetAttribute("Condition", condition.c_str());
        proj->InsertEndChild(propertyGroup);

        AddTextElement(&doc, propertyGroup, "OutDir", GetStr(config, "out_dir"));
        AddTextElement(&doc, propertyGroup, "IntDir", GetStr(config, "int_dir"));
    }

    // Item Definition Groups
    for (const auto& config : project["configuration"])
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
    for (const auto& source : project["source_files"])
	{
        //cout << source << endl;
        string str = source.get<string>();
        XMLElement* clCompile = doc.NewElement("ClCompile");
        clCompile->SetAttribute("Include", str.c_str());
        itemGroupCompile->InsertEndChild(clCompile);
        if (str.ends_with("pch.cpp"))
		{
			for (const auto& config : project["configuration"])
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
    for (const auto& header : project["header_files"])
	{
        string str = header.get<string>();
        XMLElement* clInclude = doc.NewElement("ClInclude");
        clInclude->SetAttribute("Include", str.c_str());
        itemGroupInclude->InsertEndChild(clInclude);
    }

    // Item Groups for resources files
    XMLElement* itemGroupNone = doc.NewElement("ItemGroup");
    proj->InsertEndChild(itemGroupNone);
    for (const auto& file : project["resource_files"])
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
    string folderPath = s_vsPath + project["folder"].get<string>();
    string filePath = folderPath + project["name"].get<string>() + ".vcxproj";
    fs::create_directories(folderPath);
    XMLError eResult = doc.SaveFile(filePath.c_str());
    if (eResult != XML_SUCCESS)
	{
        cerr << RED << "Error saving file: " << eResult << RESET << endl;
        //return 1;
    }

    std::cout <<  fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
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

    unordered_map<string, string> filters = {
        {"Source Files", s_srcExt},
        {"Header Files", s_hExt},
        {"Resource Files", s_rcExt} };

    // Create filters
    for (const auto& pair : filters)
	{
        XMLElement* filterElement = doc.NewElement("Filter");
        filterElement->SetAttribute("Include", pair.first.c_str());
        itemGroup->InsertEndChild(filterElement);

        XMLElement* elem = doc.NewElement("UniqueIdentifier");
        elem->SetText(GenerateGuid().c_str());
        filterElement->InsertEndChild(elem);

        elem = doc.NewElement("Extensions");
        elem->SetText(pair.second.c_str());
        filterElement->InsertEndChild(elem);
    }

    // Add files to their respective filters
    itemGroup = doc.NewElement("ItemGroup");
    proj->InsertEndChild(itemGroup);
    AddFilesToItemGroup(&doc, itemGroup, "ClCompile", project["source_files"], "Source Files");
    AddFilesToItemGroup(&doc, itemGroup, "ClInclude", project["header_files"], "Header Files");
    AddFilesToItemGroup(&doc, itemGroup, "None", project["resource_files"], "Resource Files");

    // Save the vcxproj file
    string folderPath = s_vsPath + project["folder"].get<string>();
    string filePath = folderPath + project["name"].get<string>() + ".vcxproj.filters";
    fs::create_directories(folderPath);
    XMLError eResult = doc.SaveFile(filePath.c_str());
    if (eResult != XML_SUCCESS) {
        cerr << RED << "Error saving file: " << eResult << RESET << endl;
        //return 1;
    }

    std::cout << fs::absolute(filePath) << GREEN << " generated successfully!" << RESET << endl;
}

json GCSolutionGenerator::ReadJsonFile(string fileName)
{
    ifstream inputFile(fileName);

    if (inputFile.is_open()==false)
	{
        cerr << RED << "Could not open the file " << fileName << RESET << endl;
        return nullptr;
    }

    json data;
    inputFile >> data;
    inputFile.close();

    return data;
}

void GCSolutionGenerator::FillDataWithSolAndPrj()
{
    json data = json::object();

    std::ifstream inputPath(s_solFolder);
    std::cout << "Reading data from " << s_solFolder << std::endl;
    if (inputPath.is_open()==false)
	{
        std::cerr << "Could not open the file " << s_solFolder << std::endl;
        return;
    }
        
    data = ReadJsonFile(s_solFolder);
    inputPath.close();

	data["folders"] = json::array();
	data["contents"] = json::array();
	for ( auto project : data["projects"] )
	{
		string name = project.get<string>();
        json file_data = ReadJsonFile(fs::path(s_solFolder).parent_path().string() + "/" + name);
        data["contents"].push_back(file_data);

		json folder = json::object();
		folder["name"] = name.substr(0, name.length()-4);
		data["folders"].push_back(folder);		
	} 

    m_data = data;
}

void GCSolutionGenerator::PopulateIncludeFiles(json& project)
{
    unordered_set<string> srcExt = SplitString(s_srcExt, ';');
    unordered_set<string> hExt = SplitString(s_hExt, ';');
    unordered_set<string> rcExt = SplitString(s_rcExt, ';');

    array<string, 4> filters = { "source_files", "header_files", "resource_files", "other_files" };
    // Create the filters
    for (const string& filter : filters)
	{
        if (project.find(filter) == project.end())
			project[filter] = json::array();
    }

    string path = s_srcPath + "/" + project["folder"].get<string>();
    if (fs::exists(path))
	{
        for (const auto& entry : fs::recursive_directory_iterator(path))
		{
            if (fs::is_regular_file(entry))
			{
                //cout << "Fichier : " << entry.path().string() << endl;
                string ext = entry.path().extension().string().substr(1);
                string relPath = RelativePath(s_vsPath + project["folder"].get<string>(), entry.path().string());
                bool nowhere = true;

                if (auto it = find(srcExt.begin(), srcExt.end(), ext) != srcExt.end()) {
					project["source_files"].push_back(relPath);
                    nowhere = false;
				}
                if (auto it = find(hExt.begin(), hExt.end(), ext) != hExt.end()) {
					project["header_files"].push_back(relPath);
                    nowhere = false;
				}
                if (auto it = find(rcExt.begin(), rcExt.end(), ext) != rcExt.end()) {
					project["resource_files"].push_back(relPath);
                    nowhere = false;
				}
                if(nowhere)
					project["other_files"].push_back(relPath);
            }
        }
    }
    else
	{
        fs::create_directories(path);
	}
}

string GCSolutionGenerator::GenerateGuid()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
	{
        cerr << RED << "Erreur lors de l'initialisation de COM" << RESET << endl;
        exit(1);
    }

    GUID guid;
    HRESULT hCreateGuid = CoCreateGuid(&guid); 
    
    if (hCreateGuid != S_OK) {
        cerr << RED << "Erreur lors de la generation du GUID" << RESET << endl;
        exit(1);
    }

    // Convertir le GUID en une chaine de caracteres
    wchar_t wszGuid[40] = { 0 };
    StringFromGUID2(guid, wszGuid, 40);

    // Convertir la chaine wide char en string
    char szGuid[40] = { 0 };
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, szGuid, sizeof(szGuid), wszGuid, _TRUNCATE);

    // Nettoyer COM
    CoUninitialize();

    return string(szGuid);
}

unordered_set<string> GCSolutionGenerator::SplitString(string str, char delimiter)
{
    unordered_set<string> result;
    istringstream iss(str);
    string token;
    while (getline(iss, token, delimiter)) {
        result.insert(token);
    }
    return result;
}

string GCSolutionGenerator::RelativePath(const string& from, const string& to) {
    // Résoudre les chemins absolus pour éviter les problèmes de points de départ
    fs::path absFrom = fs::absolute(from);
    fs::path absTo = fs::absolute(to);

    if(!fs::exists(absFrom))
        fs::create_directories(absFrom);
    if (!fs::exists(absTo))
        fs::create_directories(absTo);

    // Convertir en chemins canoniques pour normaliser les chemins et gérer les points de départ différents
    absFrom = fs::canonical(absFrom); 
    absTo = fs::canonical(absTo);

    // Obtenir les itérateurs pour les chemins
    auto itFrom = absFrom.begin();
    auto itTo = absTo.begin();

    // Trouver le point de divergence
    while (itFrom != absFrom.end() && itTo != absTo.end() && *itFrom == *itTo) {
        ++itFrom;
        ++itTo;
    }

    // Construire le chemin relatif à partir du point de divergence
    fs::path result;
    for (; itFrom != absFrom.end(); ++itFrom) {
        if ((*itFrom) != ".")
            result /= "..";
    }
    for (; itTo != absTo.end(); ++itTo) {
        result /= *itTo;
    }

    return result.string();
}

void GCSolutionGenerator::AddTextElement(tinyxml2::XMLDocument* doc, XMLElement* parent, const string name, const string value)
{
    if(value.empty()) return;
    XMLElement* element = doc->NewElement(name.c_str());
	element->SetText(value.c_str());
	parent->InsertEndChild(element);
}

void GCSolutionGenerator::AddFilesToItemGroup(tinyxml2::XMLDocument* doc, XMLElement* itemGroup, const string tag, json files, const string filterName)
{
    for (const auto& file : files)
	{
        XMLElement* element = doc->NewElement(tag.c_str());
        string str = file.get<string>();
        element->SetAttribute("Include", str.c_str());
        itemGroup->InsertEndChild(element);

        XMLElement* filterElement = doc->NewElement("Filter");
        filterElement->SetText(filterName.c_str());
        element->InsertEndChild(filterElement);
    }
}

string GCSolutionGenerator::GetStr(json obj, string key)
{
    if (obj.contains(key))
		return obj[key].get<string>();
    return "";
}

bool GCSolutionGenerator::DeleteFolderSafe(string path)
{
    if (fs::exists(path)) {
        unordered_set<string> srcExt = SplitString(s_srcExt, ';');
        unordered_set<string> hExt = SplitString(s_hExt, ';');
        unordered_set<string> rcExt = SplitString(s_rcExt, ';');

        array<string, 3> excludeDirs = {".vs", "Build", "vcpkg_installed"};
        fs::path fsPath;
        if (path.back() == '/') 
            fsPath = path.substr(0, path.size() - 1);
        else
            fsPath = path;

        fsPath = fsPath.filename();

        bool canDelete = true;
        if(find(excludeDirs.begin(), excludeDirs.end(), fsPath) == excludeDirs.end())
            for (const auto& entry : fs::directory_iterator(path)) {

                if (fs::is_directory(entry)) {
                    //cout << "Folder: " << entry.path().string() << endl;
                    bool tmpCanDelete = DeleteFolderSafe(entry.path().string());
                    if (tmpCanDelete == false)
                        canDelete = false;
                }
                if (fs::is_regular_file(entry)) {
                    //cout << "File: " << entry.path().string() << endl;
                    string ext = entry.path().extension().string();
                    if (!ext.empty()) {
                        ext = ext.substr(1);

                        auto itSrc = find(srcExt.begin(), srcExt.end(), ext);
                        auto itH = find(hExt.begin(), hExt.end(), ext);
                        auto itRc = find(rcExt.begin(), rcExt.end(), ext);
                        bool filePresent = itSrc != srcExt.end() || itH != hExt.end() || itRc != rcExt.end();

                        if (filePresent) {
                            std::cout << YELLOW << "WARNING: the source file " << entry.path() << " is still present and won't be deleted." << " Do you want to move it to the src folder ?" << RESET << endl;
                            char yesNo = 0;
                            while (yesNo != 'y' && yesNo != 'n') {
                                std::cout << "y/n : ";
                                cin >> yesNo;
                                yesNo = tolower(yesNo);
                            }
                            if (yesNo == 'y') {
                                bool hasMoved = MoveFileToSource(entry.path().string());
                                if (hasMoved == false)
                                    canDelete = false;
                            }
                            else {
                                canDelete = false;
                            }
                        }
                    }
                }
            }

        if (canDelete)
		{
            //cout << "Delete: " << path << endl;
            error_code ec;
            fs::remove_all(path, ec);
            if (ec) {
                cerr << RED << "Failed to remove directory: " << ec.message() << RESET << endl;
            }
            return true;
        }
    }
    return false;
}

bool GCSolutionGenerator::MoveFileToSource(string path) 
{
    string str = RelativePath(s_vsPath, path);
    fs::path source = fs::absolute(path);
    fs::path destination = fs::absolute(s_srcPath + str);

    try
	{
        if (!fs::exists(destination.parent_path()))
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

void GCSolutionGenerator::WriteJsonFile(const string& fileName, const ordered_json& data)
{
    ofstream outputFile(fileName);
    if (outputFile.is_open())
    {
        outputFile << data.dump(4);
        outputFile.close();
    }
    else
    {
        cerr << "Unable to open file for writing: " << fileName << endl;
    }
}

const std::string GCSolutionGenerator::FindFirstSolFile(const std::string& directory)
{
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (entry.path().extension() == ".sol") 
            return entry.path().string();
    }
    return "";
}
