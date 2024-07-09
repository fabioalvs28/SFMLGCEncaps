#include "GCSolutionGenerator.h"
#include <iostream>
#include <fstream>
#define NOMINMAX
#include <windows.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void EnableANSIColors();

int main()
{
    EnableANSIColors();

    // Read the json file
    const string solutionJson = "./json/Solution.json";
    ifstream inputFile(solutionJson);
    if (!inputFile.is_open()) {
        cerr << "Could not open the file " << solutionJson << endl;
        return 1;
    }
    json data;
    inputFile >> data;
    inputFile.close();

    // Retrieve the poles
    vector<string> polesAvailable = {"all"};
    for (const auto& pole : data["poles"].items()) 
        polesAvailable.push_back(pole.key());

    string input;
    cout << "Veuillez choisir les pôles à générer. Séparez les pôles par un espace." << endl;

    // Get args typed by the user
    unordered_set<string> args; 
    bool polesValid = false;
    while (!polesValid) {
        // Display the available poles
        cout << "Pôles disponibles : ";
        for (vector<string>::iterator it = polesAvailable.begin(); it != polesAvailable.end(); it++)
        {
            if (it == polesAvailable.begin())
                cout << GREEN << *it << RESET;
            else
                cout << ", " << GREEN << *it << RESET;
        }
        cout << endl;

        // Get user input
        cout << "Pôles : ";
        getline(cin, input);
        istringstream iss(input);
        string token;
        polesValid = true;
        args.clear();
        while (getline(iss, token, ' ')) {
            auto it = find(polesAvailable.begin(), polesAvailable.end(), token);
            if (it == polesAvailable.end()) {
                polesValid = false;
                cout << token << " n'est pas valide." << endl;
                break;
            }
            args.insert(token);
        }
    }

    cout << "Voulez-vous supprimer le dossier 'ide' avant de le regénérer ?" << endl;
    char yesNo = 0;
    while (yesNo != 'y' && yesNo != 'n') {
        cout << "y/n : ";
        cin >> yesNo;
        yesNo = tolower(yesNo);
    }

    // Generate the solution
    GCSolutionGenerator::GenerateSolution(args, true);

    return 0;
}

void EnableANSIColors() {
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