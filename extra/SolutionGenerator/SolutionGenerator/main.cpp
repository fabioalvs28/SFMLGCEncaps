#include "core.h"
#include <iostream>
#include <fstream>
#define NOMINMAX
#include <windows.h>

using namespace std;

#define VERSION "1.11"

void ShowHelpForMakeAndCreate();

int main(int argc, char* argv[])
{
	HRESULT hr = CoInitialize(NULL);
	if ( FAILED(hr) )
		return 1;

	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if ( hOut==INVALID_HANDLE_VALUE )
		return 1;
	DWORD dwMode = 0;
	if ( GetConsoleMode(hOut, &dwMode)==FALSE )
		return 1;
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
	SetConsoleOutputCP(1252);

	cout << "Solution Generator " << VERSION << endl;
	cout << "Copyright © 2024 Quest Education" << endl;

	if ( argc<2 )
	{
		cerr << "Incorrect number of arguments. Use -help to see available commands." << endl;
		return 1;
	}

	string command = argv[1];
	vector<string> args;
	for ( int i=2 ; i<argc ; i++ )
		args.push_back(argv[i]);

	bool result = false;
	if ( command=="-help" || command=="-h" )
	{
		ShowHelpForMakeAndCreate();
		result = true;
	}

	if ( command=="-create" )
	{
		GCSolutionGenerator sg;
		result = sg.CommandCreate(args);
	}

	if ( command=="-add" )
	{
		GCSolutionGenerator sg;
		result = sg.CommandAdd(args);
	}

	if ( command=="-make" )
	{
		GCSolutionGenerator sg;
		result = sg.CommandMake(args);
	}

	CoUninitialize();
	return result ? 0 : 1;
}

void ShowHelpForMakeAndCreate()
{
	cout << "Usage: SolutionGenerator [options]" << endl;
	cout << "Options:" << endl;
	cout << "\t-create <repo_folder> <solution_name> <project_name> {-windows} {-lib}" << endl;
	cout << "\t\trepo_folder\t\tThe main folder" << endl;
	cout << "\t\tsolution_name\t\tThe new solution name" << endl;
	cout << "\t\tproject_name\t\tThe new project name" << endl;
	cout << "\t\t-windows\t\tWindows project (optional)" << endl;
	cout << "\t\t-lib\t\t\tStatic library (optional)" << endl;
	cout << endl;
	cout << "\t-add <repo_folder> <project_name> {-windows} {-lib}" << endl;
	cout << "\t\trepo_folder\t\tThe main folder" << endl;
	cout << "\t\tproject_name\t\tThe new project name" << endl;
	cout << "\t\t-windows\t\tWindows project (optional)" << endl;
	cout << "\t\t-lib\t\t\tStatic library (optional)" << endl;
	cout << endl;
	cout << "\t-make <root_folder> {-clean}" << endl;
	cout << "\t\trepo_folder\t\tThe main folder" << endl;
	cout << "\t\t-clean\t\t\tRemove existing files (optional)" << endl;
	cout << endl;
	cout << "\t-help" << endl;
	cout << endl;
}
