@echo off
echo === Building Game ===

g++ src\main.cpp -o build\Game.exe -O2 -Wall -Wno-missing-braces -I include -L lib -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% equ 0 (
    echo === Build Successful ===
) else (
    echo === Build Failed ===
)

pause
