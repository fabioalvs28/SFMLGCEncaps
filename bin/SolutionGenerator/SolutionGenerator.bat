@echo off
echo Veuillez choisir les poles a generer. Separez les poles par un espace.
echo Poles disponibles : all, engine, gameplay, render, tools, qa

:launch
set /p poles=Poles : 
python ./src/Main.py --pole %poles%
if %ERRORLEVEL% neq 0 (
    if %ERRORLEVEL% equ 10 (
        goto :launch
    )
    pause
)