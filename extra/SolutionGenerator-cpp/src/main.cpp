#include "GCSolutionGenerator.h"
#include <iostream>
#include <fstream>
#define NOMINMAX
#include <windows.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;


void EnableANSIColors();
void ShowHelpForMakeAndCreate();

int main(int argc, char* argv[])
{
    EnableANSIColors();
    if (argc < 2)
    {
        cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
        return 1;
	}

    string command = argv[1];

    if (command == "-help")
    {
        ShowHelpForMakeAndCreate();
    }

    if (command == "-create")
    {
        if (argc != 5)
        {
            cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
            return 1;
		}
		string parent_path = argv[2];
        string solution_name = argv[3];
        string project_name = argv[4];

        string solutionFileName = parent_path + "/" +"config/" + solution_name + ".sol";
        string projectFileName = parent_path + "/" + "config/" + project_name + ".prj";


        string s_srcPath = parent_path + "/src";
        string s_idePath = parent_path + "/ide";
        string s_vsPath = s_idePath + "/vs/";

        // Create project folder
        if (!fs::exists(parent_path))
        {
            fs::create_directories(parent_path);
        }
        if (!fs::exists(parent_path + "/config"))
        {
            fs::create_directories(parent_path + "/" + "config");
        }
        if (!fs::exists(s_idePath))
        {
            fs::create_directories(s_idePath);
        }
        if (!fs::exists(s_vsPath))
        {
            fs::create_directories(s_vsPath);
        }
        if (!fs::exists(s_srcPath))
        {
            fs::create_directories(s_srcPath);
        }
        if (!fs::exists(s_srcPath + "/" + project_name))
        {
            fs::create_directories(s_srcPath + "/" + project_name);
            fs::path cppPath = s_srcPath + "/" + project_name + "/main.cpp";
            ofstream cppFile(cppPath);
            fs::path hPath = s_srcPath + "/" + project_name + "/main.h";
            ofstream hFile(hPath);
            fs::path pchPath = s_srcPath + "/" + project_name + "/pch.h";
            ofstream pchFile(pchPath);
        }
        if (!fs::exists(parent_path + "/bin"))
        {
            fs::create_directories(parent_path + "/bin");
        }
        if (!fs::exists(parent_path + "/res"))
        {
            fs::create_directories(parent_path + "/res");
        }
        if (!fs::exists(parent_path + "/doc"))
        {
            fs::create_directories(parent_path + "/doc");
        }

		// Read the json file
		ifstream inputFile(parent_path + "/" + "config");
        if (!inputFile.is_open()) 
        {
            GCSolutionGenerator::EnsureJsonFileExists(solutionFileName, projectFileName, project_name, solution_name);
		}
        json data;
        ifstream newInputFile(solutionFileName);
        newInputFile >> data;
        newInputFile.close();

        unordered_map<string, string> args =
		{
            {"parent_path", parent_path},
            {"solution_name", solution_name},
            {"project_name", project_name}
        };

        string token = "all";
        args.insert({ "all", token });

        GCSolutionGenerator::GenerateNewSolution(args);
    }

    if (command == "-makeOld")
    {
        if (argc != 4)
        {
            cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
            return 1;
        }
        string json_directory = argv[2];
        string forceRemove = argv[3];

        bool isForceRemove = false;
        if (forceRemove != "-clean")
        {
            isForceRemove = true;
        }

        std::string solution_file_path = GCSolutionGenerator::FindSolutionJSONFile(json_directory);
        if (solution_file_path.empty())
        {
            std::cerr << "No Solution json file found in the directory " << json_directory << std::endl;
            return 1;
        }

        unordered_map<string, string> args =
        {
			{"json_path", solution_file_path},
		};

        string token = "all";
        args.insert({ "all", token });

        GCSolutionGenerator::GenerateSolution(args, isForceRemove);

        return 0;
    }

    if (command == "-make")
    {
        if (argc != 3)
        {
            cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
            return 1;
        }
        string root_folder = argv[2];

        bool isForceRemove = true;

        string config_folder = root_folder + "/config";
        std::string sol_folder = GCSolutionGenerator::FindFirstSolFile(config_folder);
        if (sol_folder.empty())
        {
            std::cerr << "No .sol file found in the folder " << config_folder << std::endl;
            return 1;
        }

        unordered_map<string, string> args =
        {
            {"root_folder", root_folder},
            {"config_folder", config_folder},
			{"sol_folder", sol_folder}
        };

        string token = "all";
        args.insert({ "all", token });

        GCSolutionGenerator::GenerateSolutionSolAndPrj(args, isForceRemove);

        return 0;
    }

    return 1;
}

void EnableANSIColors() 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Enable ANSI encoding in console in order to have accents
    SetConsoleOutputCP(1252);
}

void ShowHelpForMakeAndCreate()
{
	cout << "Usage: GCSolutionGenerator [options]" << endl;
	cout << "Options:" << endl;
    cout << "\t-makeOld <json_folder> <forceRemove(-clean)>" << endl;
    cout << endl;
    cout << "\t\t <json_folder> \t\t\t: Generate the solution using the folder containing .json solution" << endl;
    cout << "\t\t <forceRemove(-clean)> \t\t: if you want to regenerate the old ide folder." << endl;
    cout << endl;
    cout << "\t\t\tTHE NEXT MADE IS FOR .SOL && .PRJ FILES" << endl;
    cout << endl;
    cout << "\t-make <root_folder> <forceRemove(-clean)> "<< endl;
    cout << endl;
    cout << "\t\t <root_folder>  \t\t\t: Generate the solution using the root folder " << endl;
    cout << endl;
    cout << "\t-create <repo_name> <solution_name> <project_name> " << endl;
    cout << endl;
    cout << "\t\t <repo_name> \t\t\t: Create a new project using the master folder name(Will take the project root by default)" << endl;
    cout << "\t\t <solution_name> \t\t: The solution's name" << endl;
    cout << "\t\t <project_name> \t\t: The new project name" << endl;
    cout << endl;
    cout << "\t-help \t\t\t\t\t:Show this help message." << endl;
    cout << endl;
}