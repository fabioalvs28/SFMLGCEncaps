import json
import argparse
import SolutionGenerator as Gen
import os
import subprocess

# paths
# script_path = os.path.dirname(os.path.realpath(__file__))
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

# parser for reading arguments
parser = argparse.ArgumentParser(description="Generate visual solution for the Gaming Campus Engine project.")
parser.add_argument('--pole', type=str, help='Pôle')
args = parser.parse_args()

# read the solution file
data = read_json_file(json_path + 'Solution.json')
pole = args.pole
if(pole not in data['poles']):
    print("Pôle invalide, pôles disponibles : " + ", ".join(data['poles']))
    exit()

data['projects'] = []

# read files depending on the pole
if pole == "all":  
    data['folders'] = []
    all_files = []
    for p in data['poles']:
        if p != "all":
            for file in data['poles'][p]["projects"]:
                all_files.append(file)
            if data['poles'][p].get('folders'):
                data['folders'] += data['poles'][p]["folders"]

    all_files = set(all_files)
    for file in all_files:
        file_data = read_json_file(json_path + file)
        data['projects'].append(file_data)
else:
    for file in data['poles'][pole]["projects"]:
        file_data = read_json_file(json_path + file)
        data['projects'].append(file_data)

    if data['poles'][pole].get('folders'):
        data['folders'] = data['poles'][pole]["folders"]

app_path = Gen.generate_solution(data)
open_file_explorer(app_path)