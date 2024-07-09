#include "GCSolutionGenerator.h"
#include <fstream>
#include <iostream>
#include <windows.h>
#include <objbase.h>
#include <filesystem>
#include <unordered_set>

using json = nlohmann::json;
using namespace tinyxml2;
using namespace std; 
namespace fs = filesystem;

// Paths 
const string GCSolutionGenerator::s_root = "../../";
const string GCSolutionGenerator::s_jsonPath = "./json/";
const string GCSolutionGenerator::s_idePath = s_root + "ide/";
const string GCSolutionGenerator::s_srcPath = s_root + "src/";
const string GCSolutionGenerator::s_vsPath = s_idePath + "vs/";
// Extensions
const string GCSolutionGenerator::s_srcExt = "cpp;c";
const string GCSolutionGenerator::s_hExt = "h;hpp";
const string GCSolutionGenerator::s_rcExt = "hlsl;jpg;jpeg;png;wav;mp3;ico;dds;rc;obj";
// Data
json GCSolutionGenerator::m_data;
unordered_set<string> GCSolutionGenerator::m_args;

void GCSolutionGenerator::GenerateSolution(unordered_set<string> args)
{
    m_args = args;

    // Get Data
    FillData();
    // Delete folder without deleting source files
    DeleteFolderSafe(s_idePath);
    
    // Generate
    GenerateSln();
    for (auto& project : m_data["projects"]) {
	    PopulateIncludeFiles(project);
	    GenerateVcxproj(project);
	    GenerateFilters(project);
	}

    // Opens explorer window
    ShellExecuteA(NULL, "open", fs::absolute(s_vsPath).string().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void GCSolutionGenerator::GenerateSln()
{
    string solutionName = m_data["solution_name"];
    json& projects = m_data["projects"];
    json& folders = m_data["folders"];

    const map<string, string> typeGuid = {
		{"project", "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"},
		{"folder", "{2150E333-8FDC-42A3-9474-1A3956D46DE8}"}
	};

    json guids = {
        {"projects", json::object()},
        {"folders", json::object()}
    };

    for(auto& project : projects) {
        string guid = GenerateGuid();
        string projectName = project["name"];
        project["guid"] = guid;
        guids["projects"][projectName] = guid;
	}

    for (auto& folder : folders) {
        string guid = GenerateGuid();
        string folderName = folder["name"];
        folder["guid"] = guid;
        guids["folders"][folderName] = guid;
    }
    //cout << guids.dump(2) << endl;

    // Create a file and write into it
    fs::path filePath = s_vsPath + solutionName + ".sln";
    fs::create_directories(s_vsPath);

    ofstream outputFile(filePath);
    if (!outputFile.is_open()) {
        cerr << "Failed to create file." << filePath << endl;
        exit(1);
    }

    outputFile << endl;
    outputFile << "Microsoft Visual Studio Solution File, Format Version " << m_data["format_version"].get<string>() << endl;
    outputFile << "# Visual Studio Version " << m_data["version"].get<string>() << endl;
    outputFile << "VisualStudioVersion = " << m_data["version_full"].get<string>() << endl;
    outputFile << "MinimumVisualStudioVersion = " << m_data["minimum_version"].get<string>() << endl;

    // Add the projects
    for (const auto& project : projects) {
        //cout << project.dump(1) << endl;
        outputFile << "Project(\"" << typeGuid.at("project") << "\") = " << project["name"] << ", \"" << project["folder"].get<string>() << project["name"].get<string>() << ".vcxproj" << "\", " << project["guid"] << endl;

        // Add the dependencies
        if (project.find("dependencies") != project.end()) {
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
    for (const auto& folder : folders) {
        outputFile << "Project(\"" << typeGuid.at("folder") << "\") = " << folder["name"] << ", " << folder["name"] << ", " << folder["guid"] << endl;
        outputFile << "EndProject" << endl;
    }

    // Add the global section
    outputFile << "Global" << endl;
    outputFile << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution" << endl;
    outputFile << "\t\tDebug|x64 = Debug|x64" << endl;
    outputFile << "\t\tRelease|x64 = Release|x64" << endl;
    outputFile << "\tEndGlobalSection" << endl;

    // Add project configurations
    outputFile << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution" << endl;
    for (const auto& project : projects) {
		outputFile << "\t\t" << project["guid"].get<string>() << ".Debug|x64.ActiveCfg = Debug|x64" << endl;
		outputFile << "\t\t" << project["guid"].get<string>() << ".Debug|x64.Build.0 = Debug|x64" << endl;
		outputFile << "\t\t" << project["guid"].get<string>() << ".Release|x64.ActiveCfg = Release|x64" << endl;
		outputFile << "\t\t" << project["guid"].get<string>() << ".Release|x64.Build.0 = Release|x64" << endl;
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
    for (const auto& folder : folders) {
        if (folder.find("nested") != folder.end()) {
            string guid = folder["guid"].get<string>();
            string nested = guids["folders"][folder["nested"]].get<string>();
            outputFile << "\t\t" << guid << " = " << nested << endl;
        }
    }
    outputFile << "\tEndGlobalSection" << endl;

    outputFile << "\tGlobalSection(ExtensibilityGlobals) = postSolution" << endl;
    outputFile << "\t\tSolutionGuid = " << GenerateGuid() << endl;
    outputFile << "\tEndGlobalSection" << endl;

    outputFile << "EndGlobal" << endl;

    outputFile.close();

    cout << fs::absolute(filePath) << " generated successfully!" << endl;
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

    array<string, 2> configs = { "Debug", "Release" };
    for (const string& config : configs) {
		XMLElement* projectConfig = doc.NewElement("ProjectConfiguration");
		projectConfig->SetAttribute("Include", (config + "|x64").c_str());
		itemGroup->InsertEndChild(projectConfig);

        AddTextElement(&doc, projectConfig, "Configuration", config);
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
    for (const auto& config : project["configuration"]) {
        string condition = "'$(Configuration)|$(Platform)'=='" + GetStr(config, "name") + "'";
        XMLElement* propertyGroup = doc.NewElement("PropertyGroup");
        propertyGroup->SetAttribute("Condition", condition.c_str());
        proj->InsertEndChild(propertyGroup);

        AddTextElement(&doc, propertyGroup, "OutDir", GetStr(config, "out_dir"));
        AddTextElement(&doc, propertyGroup, "IntDir", GetStr(config, "int_dir"));
    }

    // Item Definition Groups
    for (const auto& config : project["configuration"]) {
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
    for (const auto& source : project["source_files"]) {
        //cout << source << endl;
        string str = source.get<string>();
        XMLElement* clCompile = doc.NewElement("ClCompile");
        clCompile->SetAttribute("Include", str.c_str());
        itemGroupCompile->InsertEndChild(clCompile);
        if (str.ends_with("pch.cpp")) {
            XMLElement* precompiledHeader = doc.NewElement("PrecompiledHeader");
            precompiledHeader->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Debug|x64'");
            precompiledHeader->SetText("Create");
            clCompile->InsertEndChild(precompiledHeader);

            precompiledHeader = doc.NewElement("PrecompiledHeader");
            precompiledHeader->SetAttribute("Condition", "'$(Configuration)|$(Platform)'=='Release|x64'");
            precompiledHeader->SetText("Create");
            clCompile->InsertEndChild(precompiledHeader);
        }
    }

    // Item Groups for header files
    XMLElement* itemGroupInclude = doc.NewElement("ItemGroup");
    proj->InsertEndChild(itemGroupInclude);
    for (const auto& header : project["header_files"]) {
        string str = header.get<string>();
        XMLElement* clInclude = doc.NewElement("ClInclude");
        clInclude->SetAttribute("Include", str.c_str());
        itemGroupInclude->InsertEndChild(clInclude);
    }

    // Item Groups for resources files
    XMLElement* itemGroupNone = doc.NewElement("ItemGroup");
    proj->InsertEndChild(itemGroupNone);
    for (const auto& file : project["resource_files"]) {
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
    if (eResult != XML_SUCCESS) {
        cerr << "Error saving file: " << eResult << endl;
        //return 1;
    }

    cout << fs::absolute(filePath) << " generated successfully!" << endl;
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
    for (const auto& pair : filters) {
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
        cerr << "Error saving file: " << eResult << endl;
        //return 1;
    }

    cout << fs::absolute(filePath) << " generated successfully!" << endl;
}

json GCSolutionGenerator::ReadJsonFile(string fileName)
{
    ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        cerr << "Could not open the file!" << endl;
        return nullptr;
    }

    json data;
    inputFile >> data;
    inputFile.close();

    return data;
}

void GCSolutionGenerator::FillData()
{
    json data = ReadJsonFile(s_jsonPath + "Solution.json");
    data["all_projects"] = json::array();
    data["all_folders"] = json::array();

    // read files depending on the pole
    if (m_args.find("all") != m_args.end())
    {
        for (const auto& pole : data["poles"])
        {
            for (const auto& project : pole["projects"])
            {
				data["all_projects"].push_back(project);
                //cout << project.dump() << endl;
			}
            if (pole.find("folders") != pole.end()) {
                for (const auto& folder : pole["folders"])
                {
                    data["all_folders"].push_back(folder);
                    //cout << folder.dump() << endl;
                }
            }
        }
    }
    else {
        for (const auto& pole : m_args) {
            for (const auto& project : data["poles"][pole]["projects"]) {
                data["all_projects"].push_back(project);
            }
            json tmpPole = data["poles"][pole];
            if (tmpPole.find("folders") != tmpPole.end()) {
                for (const auto& folder : tmpPole["folders"])
                {
                    data["all_folders"].push_back(folder);
                    //cout << folder.dump() << endl;
                }
            }
        }
    }

    // delete the duplicates projects 
    unordered_set<string> uniqueProjects;
    for (const auto& project : data["all_projects"]) {
        uniqueProjects.insert(project);
    }
    data.erase("all_projects");

    for (const auto& project : uniqueProjects) {
        json file_data = ReadJsonFile(s_jsonPath + project);
        data["projects"] += file_data;
    }

    // delete the duplicates folders 
    unordered_set<json> uniqueFolders;
    for (const auto& folder : data["all_folders"]) {
        uniqueFolders.insert(folder);
    }
    data.erase("all_folders");

    for (const auto& folder : uniqueFolders) {
        data["folders"] += folder;
    }
    //cout << data["folders"].dump(2) << endl;

    //cout << data.dump(2) << endl;
    m_data = data;
}

void GCSolutionGenerator::PopulateIncludeFiles(json& project)
{
    unordered_set<string> srcExt = SplitString(s_srcExt, ';');
    unordered_set<string> hExt = SplitString(s_hExt, ';');
    unordered_set<string> rcExt = SplitString(s_rcExt, ';');

    array<string, 4> filters = { "source_files", "header_files", "resource_files", "other_files" };
    // Create the filters
    for (const string& filter : filters) {
        if (project.find(filter) == project.end()) {
			project[filter] = json::array();
		}
    }

    string path = s_srcPath + project["folder"].get<string>();
    if (fs::exists(path)) {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry)) {
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
                if(nowhere) {
					project["other_files"].push_back(relPath);
				}
            }
        }
    }
    else {
        fs::create_directories(path);
	}

    //cout << project.dump(2) << endl;
}

string GCSolutionGenerator::GenerateGuid()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) {
        cerr << "Erreur lors de l'initialisation de COM" << endl;
        exit(1);
    }

    GUID guid;
    HRESULT hCreateGuid = CoCreateGuid(&guid); 
    
    if (hCreateGuid != S_OK) {
        cerr << "Erreur lors de la g�n�ration du GUID" << endl;
        return nullptr;
    }

    // Convertir le GUID en une cha�ne de caract�res
    wchar_t wszGuid[40] = { 0 };
    StringFromGUID2(guid, wszGuid, 40);

    // Convertir la cha�ne wide char en string
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
    for (const auto& file : files) {
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
    if (obj.contains(key)) {
		return obj[key].get<string>();
	}
    return "";
}

bool GCSolutionGenerator::DeleteFolderSafe(string path)
{
    if (fs::exists(path)) {
        unordered_set<string> srcExt = SplitString(s_srcExt, ';');
        unordered_set<string> hExt = SplitString(s_hExt, ';');
        unordered_set<string> rcExt = SplitString(s_rcExt, ';');

        array<string, 2> excludeDirs = { ".vs", "Build" };
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
                            canDelete = false;
                            cout << "WARNING: the source file " << entry.path() << " is still present. The folder won't be deleted." << endl;
                        }
                    }
                }
            }

        if (canDelete) {
            //cout << "Delete: " << path << endl;
            error_code ec;
            fs::remove_all(path, ec);
            if (ec) {
                cerr << "Failed to remove directory: " << ec.message() << endl;
            }
            return true;
        }
    }
    return false;
}
