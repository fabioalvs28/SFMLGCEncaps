import json
import argparse
import SolutionGenerator as Gen
import os
import subprocess

# paths
json_path = "./json/"

def read_json_file(file_path):
    # print(f"Reading file: {file_path}")
    with open(file_path, 'r') as file:
        data = json.load(file)
    return data

def open_file_explorer(folder_path):
    try:
        # Check if folder exists
        if not os.path.isdir(folder_path):
            print(f"Le dossier {folder_path} n'existe pas.")
            return
        
        # Open the folder in the file explorer
        subprocess.run(['explorer', folder_path], shell=True)
        print(f"L'explorateur de fichiers a été ouvert sur {folder_path}.")
    
    except Exception as e:
        print(f"Une erreur est survenue : {e}")

def construct_data():
    data = read_json_file(json_path + 'Solution.json')
    data['projects'] = []
    data['folders'] = []
    data['all_projects'] = []

    def add_projects(projects):
        data['all_projects'].extend(projects)
        data['all_projects'] = list(set(data['all_projects']))

    def add_folders(folders):
        data['folders'].extend(folders)
        # data['all_folders'] = list(set(data['all_folders']))

    # read files depending on the pole
    if "all" in poles:
        for pole in data['poles']:
            add_projects(data['poles'][pole]["projects"])
            if data['poles'][pole].get('folders'):
                add_folders(data['poles'][pole]["folders"])
    else:
        # check if the poles are valid
        for pole in poles:
            if(pole not in data['poles']):
                print(f"'{pole}' n'est pas un pôle valide, pôles disponibles : " + "all, " + ", ".join(data['poles']))
                exit(10)

        for pole in poles:
            add_projects(data['poles'][pole]["projects"])

            if data['poles'][pole].get('folders'):
                add_folders(data['poles'][pole]["folders"])

    for file in data['all_projects']:
        file_data = read_json_file(json_path + file)
        data['projects'].append(file_data)
    
    del data['all_projects']

    return data

# parser for reading arguments
parser = argparse.ArgumentParser()
parser.add_argument('--pole', type=str, nargs='+')
args = parser.parse_args()
pole = args.pole[0]
poles = args.pole

# read the solution file
data = construct_data()
app_path = Gen.generate_solution(data)
open_file_explorer(app_path)