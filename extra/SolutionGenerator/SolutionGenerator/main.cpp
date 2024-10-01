#include "core.h"
#include <iostream>
#include <fstream>
#define NOMINMAX
#include <windows.h>
#include "json.hpp"

#define VERSION		"1.1"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;
extern const char* g_contentMain;
extern const char* g_contentRc;
extern const char* g_contentRes;

void EnableANSIColors();
void ShowHelpForMakeAndCreate();

int main(int argc, char* argv[])
{
	SetConsoleOutputCP(1251);
	cout << "Solution Generator " << VERSION << endl;
	cout << "Copyright © 2024 Quest Education" << endl;

    EnableANSIColors();
    if (argc < 2)
    {
        cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
        return 1;
	}

    string command = argv[1];

    if (command == "-help" || command == "-h" )
    {
        ShowHelpForMakeAndCreate();
		return 0;
    }

    if (command == "-create")
    {
        if (argc < 5)
        {
            cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
            return 1;
		}
		string parent_path = argv[2];
        string solution_name = argv[3];
        string project_name = argv[4];
		bool windows = argc>=6 ? _stricmp(argv[5], "-windows")==0 : false;

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
			string data = "";

			if ( windows )
				data = g_contentMain;
			else
				data = "#include \"pch.h\"\n#include \"main.h\"\n\nint main()\n{\n\treturn 0;\n}\n";
            ofstream maincpp(s_srcPath + "/" + project_name + "/main.cpp");
			maincpp.write(data.c_str(), data.length());
			maincpp.close();

            ofstream mainh(s_srcPath + "/" + project_name + "/main.h");
			data = "#pragma once\n\n";
			mainh.write(data.c_str(), data.length());
			mainh.close();

            ofstream pch(s_srcPath + "/" + project_name + "/pch.h");
			if ( windows )
				data = "#pragma once\n\n#include <windows.h>\n\n#include \"resource.h\"\n\n";
			else
				data = "#pragma once\n\n";
			pch.write(data.c_str(), data.length());
			pch.close();

            ofstream pchcpp(s_srcPath + "/" + project_name + "/pch.cpp");
			data = "#include \"pch.h\"\n\n";
			pchcpp.write(data.c_str(), data.length());
			pchcpp.close();

            ofstream rc(s_srcPath + "/" + project_name + "/app.rc");
			data = g_contentRc;
			rc.write(data.c_str(), data.length());
			rc.close();

            ofstream res(s_srcPath + "/" + project_name + "/resource.h");
			data = g_contentRes;
			res.write(data.c_str(), data.length());
			res.close();

            ofstream ico(s_srcPath + "/" + project_name + "/app.ico");
			ico.close();

            ofstream icosm(s_srcPath + "/" + project_name + "/small.ico");
			icosm.close();
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
            GCSolutionGenerator::EnsureJsonFileExists(solutionFileName, projectFileName, project_name, solution_name, windows);
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

    if (command == "-make")
    {
        if ( argc<3 )
        {
            cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
            return 1;
        }
        string root_folder = argv[2];

        bool isForceRemove = false;
		if ( argc>=4 && std::string(argv[3])=="-clean" )
			isForceRemove = true;

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
    if (GetConsoleMode(hOut, &dwMode)==FALSE) {
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
    cout << "\t-create <repo_name> <solution_name> <project_name>" << endl;
    cout << endl;
    cout << "\t\t<repo_name>\t\tCreate a new project using the master folder name (project root by default)" << endl;
    cout << "\t\t<solution_name>\t\tThe new solution name" << endl;
    cout << "\t\t<project_name>\t\tThe new project name" << endl;
    cout << "\t\t-windows\t\tWindows project (optional)" << endl;
    cout << endl;
    cout << "\t-make <root_folder> {-clean}" << endl;
    cout << endl;
    cout << "\t\t<root_folder>\t\tGenerate the solution using the root folder" << endl;
    cout << "\t\t-clean\t\t\tRemove existing files (optional)" << endl;
    cout << endl;
    cout << "\t-help \t\t\t\tShow this help message." << endl;
    cout << endl;
}
