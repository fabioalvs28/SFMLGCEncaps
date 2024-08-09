@echo off
CHOICE /M "Are you sure to build ALL"
IF ERRORLEVEL ==2 GOTO END
IF ERRORLEVEL ==1 GOTO OK
GOTO END
:OK
rem Lancer le programme avec les arguments nécessaires

rem Chemin vers l'exécutable

set EXE_PATH="SolutionGenerator.exe"

rem Arguments du programme

set ARG1=-makeOld
set ARG2=../config/
set ARG5=-clean

rem Lancer l'exécutable avec les arguments

%EXE_PATH% %ARG1% %ARG2% %ARG3% %ARG4% %ARG5%
:END