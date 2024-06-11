@echo off
set /p pole=Generer le projet pour le pole (all, engine, gameplay, render, tools, qa) : 
python ./src/SolutionGenerator.py --pole %pole%
pause