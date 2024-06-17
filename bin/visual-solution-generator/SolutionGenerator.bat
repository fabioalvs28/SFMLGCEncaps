@echo off
set /p pole=Generer le projet pour le pole (all, engine, gameplay, render, tools, qa) : 
python ./src/Main.py --pole %pole%
echo The script will close in 5 seconds...
timeout /t 5 /nobreak > NUL